// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/N1CombatSet.h"

#include "AbilitySystem/Attributes/N1AttributeSet.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1CombatSet)

UN1CombatSet::UN1CombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UN1CombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UN1CombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UN1CombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UN1CombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UN1CombatSet, BaseDamage, OldValue);
}

void UN1CombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UN1CombatSet, BaseHeal, OldValue);
}