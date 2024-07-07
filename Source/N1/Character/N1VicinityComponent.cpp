// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/N1VicinityComponent.h"

#include "N1GameplayTags.h"
#include "Character/N1PawnExtensionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Inventory/N1InventoryItemDefinition.h"
#include "Inventory/N1InventoryItemInstance.h"
#include "Player/N1PlayerController.h"
#include "Player/N1PlayerState.h"
#include "Character/N1Character.h"
#include "N1LogChannels.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"

const FName UN1VicinityComponent::NAME_BindInputsNow("BindInputsNow");
const FName UN1VicinityComponent::NAME_ActorFeatureName("Vicinity");

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_N1_Vicinity_Message_StackChanged, "N1.Vicinity.Message.StackChanged");


//////////////////////////////////////////////////////////////////////
// FN1VicinityEntry
FString FN1VicinityEntry::GetDebugString() const
{
	TSubclassOf<UN1InventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FN1VicinityList

void FN1VicinityList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FN1VicinityEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FN1VicinityList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FN1VicinityEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FN1VicinityList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FN1VicinityEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FN1VicinityList::BroadcastChangeMessage(FN1VicinityEntry& Entry, int32 OldCount, int32 NewCount)
{
	FN1VicinityChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_N1_Vicinity_Message_StackChanged, Message);
}

UN1InventoryItemInstance* FN1VicinityList::AddEntry(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount)
{
	UN1InventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FN1VicinityEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UN1InventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UN1InventoryItemFragment* Fragment : GetDefault<UN1InventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const UN1InventoryItemDefinition* ItemCDO = GetDefault<UN1InventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FN1VicinityList::AddEntry(UN1InventoryItemInstance* Instance)
{
	unimplemented();
}

void FN1VicinityList::RemoveEntry(UN1InventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FN1VicinityEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UN1InventoryItemInstance*> FN1VicinityList::GetAllItems() const
{
	TArray<UN1InventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FN1VicinityEntry& Entry : Entries)
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

UN1VicinityComponent::UN1VicinityComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, VicinityList(this)
{
	SetIsReplicatedByDefault(true);
}

void UN1VicinityComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, VicinityList);
}

bool UN1VicinityComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == N1GameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == N1GameplayTags::InitState_Spawned && DesiredState == N1GameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AN1PlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AN1PlayerController* N1PC = GetController<AN1PlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !N1PC || !N1PC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AN1PlayerState* N1PS = GetPlayerState<AN1PlayerState>();

		return N1PS && Manager->HasFeatureReachedInitState(Pawn, UN1PawnExtensionComponent::NAME_ActorFeatureName, N1GameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == N1GameplayTags::InitState_DataInitialized && DesiredState == N1GameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UN1VicinityComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AN1PlayerState* N1PS = GetPlayerState<AN1PlayerState>();
		if (!ensure(Pawn && N1PS))
		{
			return;
		}

		// TODO : Intialize something
		if (AN1Character* N1Character = Cast<AN1Character>(Pawn))
		{
			N1Character->VicinityCollision->OnComponentBeginOverlap.AddDynamic(this, &UN1VicinityComponent::OnOverlapBegin);
			N1Character->VicinityCollision->OnComponentEndOverlap.AddDynamic(this, &UN1VicinityComponent::OnOverlapEnd);
		}
	}
}

void UN1VicinityComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UN1PawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == N1GameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UN1VicinityComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { N1GameplayTags::InitState_Spawned, N1GameplayTags::InitState_DataAvailable, N1GameplayTags::InitState_DataInitialized, N1GameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

bool UN1VicinityComponent::CanAddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount)
{
	return true;
}

UN1InventoryItemInstance* UN1VicinityComponent::AddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount)
{
	UN1InventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = VicinityList.AddEntry(ItemDef, StackCount);

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UN1VicinityComponent::AddItemInstance(UN1InventoryItemInstance* ItemInstance)
{
	VicinityList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UN1VicinityComponent::RemoveItemInstance(UN1InventoryItemInstance* ItemInstance)
{
	VicinityList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UN1InventoryItemInstance*> UN1VicinityComponent::GetAllItems() const
{
	return VicinityList.GetAllItems();
}

UN1InventoryItemInstance* UN1VicinityComponent::FindFirstItemStackByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef) const
{
	for (const FN1VicinityEntry& Entry : VicinityList.Entries)
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

void UN1VicinityComponent::BroadcastChangeMessage(UN1InventoryItemInstance* ItemInstance, int32 OldCount, int32 NewCount)
{
	FN1VicinityEntry Entry;
	Entry.Instance = ItemInstance;

	VicinityList.BroadcastChangeMessage(Entry, OldCount, NewCount);
}



int32 UN1VicinityComponent::GetTotalItemCountByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FN1VicinityEntry& Entry : VicinityList.Entries)
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


bool UN1VicinityComponent::ConsumeItemsByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 NumToConsume)
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
		if (UN1InventoryItemInstance* Instance = UN1VicinityComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			VicinityList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

void UN1VicinityComponent::OnChangedList()
{
	for (FN1VicinityEntry& Entry : VicinityList.Entries)
	{
		VicinityList.BroadcastChangeMessage(Entry, Entry.LastObservedCount, Entry.StackCount);
	}
}

void UN1VicinityComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogN1, Error, TEXT("[UN1VicinityComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		/*if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("N1VicinityComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("N1VicinityComponent");

			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));

			FMessageLog(HeroMessageLogName).Open();
		}*/
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

void UN1VicinityComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UN1PawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(N1GameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UN1VicinityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}