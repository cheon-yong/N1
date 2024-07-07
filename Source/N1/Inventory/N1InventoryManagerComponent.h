// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "N1InventoryManagerComponent.generated.h"

class UN1InventoryItemDefinition;
class UN1InventoryItemInstance;
class UN1InventoryManagerComponent;
class UN1VicinityComponent;
class UObject;
struct FFrame;
struct FN1InventoryList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FN1InventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UN1InventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	int32 Delta = 0;
};




/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FN1InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FN1InventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FN1InventoryList;
	friend UN1InventoryManagerComponent;
	friend UN1VicinityComponent;

	UPROPERTY()
	TObjectPtr<UN1InventoryItemInstance> Instance = nullptr;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FN1InventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FN1InventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FN1InventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UN1InventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FN1InventoryEntry, FN1InventoryList>(Entries, DeltaParms, *this);
	}

	UN1InventoryItemInstance* AddEntry(TSubclassOf<UN1InventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UN1InventoryItemInstance* Instance);

	void RemoveEntry(UN1InventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FN1InventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UN1InventoryManagerComponent;
	friend UN1VicinityComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FN1InventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FN1InventoryList> : public TStructOpsTypeTraitsBase2<FN1InventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * Manages an inventory
 */
UCLASS(BlueprintType)
class N1_API UN1InventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	public:
	UN1InventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	bool CanAddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	UN1InventoryItemInstance* AddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void AddItemInstance(UN1InventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveItemInstance(UN1InventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category = Inventory, BlueprintPure = false)
	TArray<UN1InventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category = Inventory, BlueprintPure)
	UN1InventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef) const;

	int32 GetTotalItemCountByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 NumToConsume);

	UFUNCTION(BlueprintCallable)
	void BroadcastChangeMessage(UN1InventoryItemInstance* ItemInstance, int32 OldCount, int32 NewCount);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface

private:
	UPROPERTY(Replicated)
	FN1InventoryList InventoryList;
};
