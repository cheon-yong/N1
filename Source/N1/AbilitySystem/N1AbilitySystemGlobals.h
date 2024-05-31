// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemGlobals.h"

#include "N1AbilitySystemGlobals.generated.h"

class UObject;
struct FGameplayEffectContext;

/**
 * 
 */
UCLASS(Config=Game)
class N1_API UN1AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_UCLASS_BODY()
	
	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
};
