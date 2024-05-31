// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/N1AbilitySystemGlobals.h"

#include "AbilitySystem/N1GameplayEffectContext.h"

UN1AbilitySystemGlobals::UN1AbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FGameplayEffectContext* UN1AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FN1GameplayEffectContext();
}
