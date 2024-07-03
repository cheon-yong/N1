// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "N1AbilityCost.h"
#include "ScalableFloat.h"
#include "Templates/SubclassOf.h"

#include "N1AbilityCost_InventoryItem.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class UN1GameplayAbility;
class UN1InventoryItemDefinition;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * Represents a cost that requires expending a quantity of an inventory item
 */
UCLASS(meta=(DisplayName="Inventory Item"))
class UN1AbilityCost_InventoryItem : public UN1AbilityCost
{
	GENERATED_BODY()

public:
	UN1AbilityCost_InventoryItem();

	//~UN1AbilityCost interface
	virtual bool CheckCost(const UN1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UN1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UN1AbilityCost interface

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	TSubclassOf<UN1InventoryItemDefinition> ItemDefinition;
};
