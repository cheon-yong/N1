// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/N1AttributeSet.h"

#include "AbilitySystem/N1AbilitySystemComponent.h"

UN1AttributeSet::UN1AttributeSet()
{
}

UWorld* UN1AttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UN1AbilitySystemComponent* UN1AttributeSet::GetN1AbilitySystemComponent() const
{
	return Cast<UN1AbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
