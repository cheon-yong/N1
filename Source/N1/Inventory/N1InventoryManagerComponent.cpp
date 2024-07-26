// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1InventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "N1InventoryItemDefinition.h"
#include "N1InventoryItemInstance.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1InventoryManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_N1_Inventory_Message_StackChanged, "N1.Inventory.Message.StackChanged");

//////////////////////////////////////////////////////////////////////
// FN1InventoryEntry

FString FN1InventoryEntry::GetDebugString() const
{
	TSubclassOf<UN1InventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), Instance->GetStackCounter(), *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FN1InventoryList

void FN1InventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FN1InventoryEntry& Stack = Entries[Index];
		int32 StackCount = Stack.Instance->GetStackCounter();
		BroadcastChangeMessage(Stack, /*OldCount=*/ StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FN1InventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FN1InventoryEntry& Stack = Entries[Index];
		int32 StackCount = Stack.Instance->GetStackCounter();
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ StackCount);
		Stack.LastObservedCount = StackCount;
	}
}

void FN1InventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FN1InventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		int32 StackCount = Stack.Instance->GetStackCounter();
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ StackCount);
		Stack.LastObservedCount = StackCount;
	}
}

void FN1InventoryList::BroadcastChangeMessage(FN1InventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FN1InventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_N1_Inventory_Message_StackChanged, Message);
}

UN1InventoryItemInstance* FN1InventoryList::AddEntry(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount)
{
	UN1InventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FN1InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UN1InventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UN1InventoryItemFragment* Fragment : GetDefault<UN1InventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.Instance->SetStackCounter(StackCount);
	Result = NewEntry.Instance;

	//const UN1InventoryItemDefinition* ItemCDO = GetDefault<UN1InventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FN1InventoryList::AddEntry(UN1InventoryItemInstance* Instance)
{
	FN1InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = Instance;
	MarkItemDirty(NewEntry);
}

void FN1InventoryList::RemoveEntry(UN1InventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FN1InventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UN1InventoryItemInstance*> FN1InventoryList::GetAllItems() const
{
	TArray<UN1InventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FN1InventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

//////////////////////////////////////////////////////////////////////
// UN1InventoryManagerComponent

UN1InventoryManagerComponent::UN1InventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UN1InventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool UN1InventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UN1InventoryItemInstance* UN1InventoryManagerComponent::AddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount)
{
	UN1InventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UN1InventoryManagerComponent::AddItemInstance(UN1InventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UN1InventoryManagerComponent::RemoveItemInstance(UN1InventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);
	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UN1InventoryItemInstance*> UN1InventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UN1InventoryItemInstance* UN1InventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef) const
{
	for (const FN1InventoryEntry& Entry : InventoryList.Entries)
	{
		UN1InventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UN1InventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FN1InventoryEntry& Entry : InventoryList.Entries)
	{
		UN1InventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UN1InventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UN1InventoryItemInstance* Instance = UN1InventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

bool UN1InventoryManagerComponent::ConsumeItemByDefinitionWithStack(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	if (UN1InventoryItemInstance* Instance = UN1InventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
	{
		int32 OldCount = Instance->GetStackCounter();
		Instance->ReduceStackCounter(NumToConsume);
		int32 NewCount = Instance->GetStackCounter();
		if (NewCount < 0)
		{
			NewCount = 0;
		}

		BroadcastChangeMessage(Instance, OldCount, NewCount);

		if (Instance->GetStackCounter() <= 0)
		{
			InventoryList.RemoveEntry(Instance);
		}

		

		return true;
	}
	return false;
}

void UN1InventoryManagerComponent::BroadcastChangeMessage(UN1InventoryItemInstance* ItemInstance, int32 OldCount, int32 NewCount)
{
	FN1InventoryEntry Entry;
	Entry.Instance = ItemInstance;

	InventoryList.BroadcastChangeMessage(Entry, OldCount, NewCount);
}

void UN1InventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing UN1InventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FN1InventoryEntry& Entry : InventoryList.Entries)
		{
			UN1InventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

bool UN1InventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FN1InventoryEntry& Entry : InventoryList.Entries)
	{
		UN1InventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

//////////////////////////////////////////////////////////////////////
//

// UCLASS(Abstract)
// class UN1InventoryFilter : public UObject
// {
// public:
// 	virtual bool PassesFilter(UN1InventoryItemInstance* Instance) const { return true; }
// };

// UCLASS()
// class UN1InventoryFilter_HasTag : public UN1InventoryFilter
// {
// public:
// 	virtual bool PassesFilter(UN1InventoryItemInstance* Instance) const { return true; }
// };


