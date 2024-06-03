// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/N1GameplayAbility.h"

#include "N1GameplayAbility_FromEquipment.generated.h"

class UN1EquipmentInstance;
class UN1InventoryItemInstance;

/**
 * UN1GameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class UN1GameplayAbility_FromEquipment : public UN1GameplayAbility
{
	GENERATED_BODY()

	public:

	UN1GameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "N1|Ability")
	UN1EquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "N1|Ability")
	UN1InventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
