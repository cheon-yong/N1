// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/N1GameplayAbility.h"

UN1GameplayAbility::UN1GameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EN1AbilityActivationPolicy::OnInputTriggered;

}
