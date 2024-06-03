// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1GameplayAbility_Death.h"

#include "AbilitySystem/Abilities/N1GameplayAbility.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "Character/N1HealthComponent.h"
#include "N1GameplayTags.h"
#include "N1LogChannels.h"
#include "Trace/Trace.inl"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameplayAbility_Death)

UN1GameplayAbility_Death::UN1GameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDeath = true;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = N1GameplayTags::GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UN1GameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UN1AbilitySystemComponent* N1ASC = CastChecked<UN1AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(N1GameplayTags::Ability_Behavior_SurvivesDeath);

	// Cancel all abilities and block others from starting.
	N1ASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);

	if (!ChangeActivationGroup(EN1AbilityActivationGroup::Exclusive_Blocking))
	{
		UE_LOG(LogN1AbilitySystem, Error, TEXT("UN1GameplayAbility_Death::ActivateAbility: Ability [%s] failed to change activation group to blocking."), *GetName());
	}

	if (bAutoStartDeath)
	{
		StartDeath();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UN1GameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	// Always try to finish the death when the ability ends in case the ability doesn't.
	// This won't do anything if the death hasn't been started.
	FinishDeath();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UN1GameplayAbility_Death::StartDeath()
{
	if (UN1HealthComponent* HealthComponent = UN1HealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EN1DeathState::NotDead)
		{
			HealthComponent->StartDeath();
		}
	}
}

void UN1GameplayAbility_Death::FinishDeath()
{
	if (UN1HealthComponent* HealthComponent = UN1HealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EN1DeathState::DeathStarted)
		{
			HealthComponent->FinishDeath();
		}
	}
}

