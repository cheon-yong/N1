// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "N1VicinityComponent.generated.h"


class UBoxComponent;
class UN1InventoryManagerComponent;
class UN1InventoryItemInstance;
class UN1InventoryItemDefinition;
struct FN1VicinityList;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FN1VicinityChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadWrite, Category = Vicinity)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Vicinity)
	TObjectPtr<UN1InventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Vicinity)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadWrite, Category = Vicinity)
	int32 Delta = 0;
};


USTRUCT(BlueprintType)
struct FN1VicinityEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FN1VicinityEntry()
	{}

	FString GetDebugString() const;

private:
	friend FN1VicinityList;
	friend UN1InventoryManagerComponent;
	friend UN1VicinityComponent;

	UPROPERTY()
	TObjectPtr<UN1InventoryItemInstance> Instance = nullptr;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FN1VicinityList : public FFastArraySerializer
{
	GENERATED_BODY()

	FN1VicinityList()
		: OwnerComponent(nullptr)
	{
	}

	FN1VicinityList(UActorComponent* InOwnerComponent)
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
		return FFastArraySerializer::FastArrayDeltaSerialize<FN1VicinityEntry, FN1VicinityList>(Entries, DeltaParms, *this);
	}

	UN1InventoryItemInstance* AddEntry(TSubclassOf<UN1InventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UN1InventoryItemInstance* Instance);

	void RemoveEntry(UN1InventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FN1VicinityEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UN1InventoryManagerComponent;
	friend UN1VicinityComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FN1VicinityEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FN1VicinityList> : public TStructOpsTypeTraitsBase2<FN1VicinityList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class N1_API UN1VicinityComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UN1VicinityComponent(const FObjectInitializer& ObjectInitializer);

	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNow;

	/** The name of this component-implemented feature */
	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	// ~ Begin TODO : make interface
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

	UFUNCTION(BlueprintCallable)
	void BroadcastChangeMessage(UN1InventoryItemInstance* ItemInstance, int32 OldCount, int32 NewCount);

	int32 GetTotalItemCountByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef, int32 NumToConsume);
	// ~ End TODO : make interface

	UFUNCTION(BlueprintCallable)
	void SpawnItemInstance(UN1InventoryItemInstance* InventroyInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UFUNCTION(BlueprintCallable)
	void OnChangedList();

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FN1VicinityList VicinityList;
};
