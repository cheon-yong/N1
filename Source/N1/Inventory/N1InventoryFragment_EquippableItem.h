// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "N1InventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "N1InventoryFragment_EquippableItem.generated.h"

class UN1EquipmentDefinition;


UCLASS()
class UN1InventoryItemFragment_EquippableItem : public UN1InventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = N1)
	TSubclassOf<UN1EquipmentDefinition> EquipmentDefinition;
};
