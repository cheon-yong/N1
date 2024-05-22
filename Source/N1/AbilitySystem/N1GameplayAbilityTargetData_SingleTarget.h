// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "N1GameplayAbilityTargetData_SingleTarget.generated.h"

/**
 * 
 */
USTRUCT()
struct FN1GameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
	
public:
	FN1GameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FN1GameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}
	UPROPERTY()
	int32 CartridgeID;
};
