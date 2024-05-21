// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ControllerComponent.h"
#include "N1QuickBarComponent.generated.h"

class UN1InventoryItemInstance;
class UN1EquipmentInstance;
class UN1EquipmentManagerComponent;


/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class N1_API UN1QuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	UN1QuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, UN1InventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "N1")
	void SetActiveSlotIndex(int32 NewIndex);

	UN1EquipmentManagerComponent* FindEquipmentManager() const;

	void UnequipItemInSlot();

	void EquipItemInSlot();


public:
	UPROPERTY()
	int32 NumSlots = 3;

	UPROPERTY()
	TArray<TObjectPtr<UN1InventoryItemInstance>> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UN1EquipmentInstance> EquippedItem;
};
