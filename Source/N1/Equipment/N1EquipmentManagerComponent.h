// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PawnComponent.h"
#include "N1EquipmentManagerComponent.generated.h"

class UN1EquipmentDefinition;
class UN1EquipmentInstance;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FN1AppliedEquipmentEntry
{
	GENERATED_BODY()
	
	UPROPERTY()
	TSubclassOf<UN1EquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UN1EquipmentInstance> Instance = nullptr;
};


USTRUCT(BlueprintType)
struct FN1EquipmentList
{
	GENERATED_BODY()
	
	FN1EquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	UN1EquipmentInstance* AddEntry(TSubclassOf<UN1EquipmentDefinition> EquipmentDefinition);
	
	void RemoveEntry(UN1EquipmentInstance* Instance);

	UPROPERTY()
	TArray<FN1AppliedEquipmentEntry> Entries;
	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};


UCLASS(BlueprintType)
class N1_API UN1EquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:

	UN1EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UN1EquipmentInstance* EquipItem(TSubclassOf<UN1EquipmentDefinition> EquipmentDefinition);
	
	void UnequipItem(UN1EquipmentInstance* ItemInstance);

public:
	UPROPERTY()
	FN1EquipmentList EquipmentList;

};
