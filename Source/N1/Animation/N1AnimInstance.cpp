// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/N1AnimInstance.h"
#include <AbilitySystemGlobals.h>

UN1AnimInstance::UN1AnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UN1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}

}

void UN1AnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	GameplayTagPropertyMap.Initialize(this, ASC);
}
