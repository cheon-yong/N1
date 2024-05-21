// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "N1InventoryManagerComponent.generated.h"

class UN1InventoryItemInstance;
class UN1InventoryItemDefinition;

USTRUCT(BlueprintType)
struct FN1InventoryEntry
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UN1InventoryItemInstance> Instance = nullptr;
};
USTRUCT(BlueprintType)
struct FN1InventoryList
{
	GENERATED_BODY()
	
	FN1InventoryList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	UN1InventoryItemInstance* AddEntry(TSubclassOf<UN1InventoryItemDefinition> ItemDef);

	UPROPERTY()
	TArray<FN1InventoryEntry> Entries;
	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

UCLASS(BlueprintType)
class N1_API UN1InventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UN1InventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	UN1InventoryItemInstance* AddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef);

	UPROPERTY()
	FN1InventoryList InventoryList;
};
