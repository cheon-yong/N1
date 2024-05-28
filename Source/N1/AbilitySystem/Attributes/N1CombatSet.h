// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Attributes/N1AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "N1CombatSet.generated.h"

class UObject;
struct FFrame;

/**
 * 
 */
UCLASS(BlueprintType)
class N1_API UN1CombatSet : public UN1AttributeSet
{
	GENERATED_BODY()
	
public:

	UN1CombatSet();

	ATTRIBUTE_ACCESSORS(UN1CombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UN1CombatSet, BaseHeal);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "N1|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "N1|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
