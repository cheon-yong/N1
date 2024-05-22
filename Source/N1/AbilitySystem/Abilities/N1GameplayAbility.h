// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "N1GameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EN1AbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn,
};


/**
 * 
 */
UCLASS(Abstract)
class N1_API UN1GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UN1GameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|AbilityActivation")
	EN1AbilityActivationPolicy ActivationPolicy;
};
