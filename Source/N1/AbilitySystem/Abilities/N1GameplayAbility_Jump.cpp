// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1GameplayAbility_Jump.h"

#include "AbilitySystem/Abilities/N1GameplayAbility.h"
#include "Character/N1Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameplayAbility_Jump)

struct FGameplayTagContainer;


UN1GameplayAbility_Jump::UN1GameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UN1GameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const AN1Character* N1Character = Cast<AN1Character>(ActorInfo->AvatarActor.Get());
	if (!N1Character || !N1Character->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UN1GameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UN1GameplayAbility_Jump::CharacterJumpStart()
{
	if (AN1Character* N1Character = GetN1CharacterFromActorInfo())
	{
		if (N1Character->IsLocallyControlled() && !N1Character->bPressedJump)
		{
			N1Character->UnCrouch();
			N1Character->Jump();
		}
	}
}

void UN1GameplayAbility_Jump::CharacterJumpStop()
{
	if (AN1Character* N1Character = GetN1CharacterFromActorInfo())
	{
		if (N1Character->IsLocallyControlled() && N1Character->bPressedJump)
		{
			N1Character->StopJumping();
		}
	}
}

