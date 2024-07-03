// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1AbilityCost_PlayerTagStack.h"

#include "GameFramework/Controller.h"
#include "N1GameplayAbility.h"
#include "Player/N1PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1AbilityCost_PlayerTagStack)

UN1AbilityCost_PlayerTagStack::UN1AbilityCost_PlayerTagStack()
{
	Quantity.SetValue(1.0f);
}

bool UN1AbilityCost_PlayerTagStack::CheckCost(const UN1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (AN1PlayerState* PS = Cast<AN1PlayerState>(PC->PlayerState))
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

			return PS->GetStatTagStackCount(Tag) >= NumStacks;
		}
	}
	return false;
}

void UN1AbilityCost_PlayerTagStack::ApplyCost(const UN1GameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (AN1PlayerState* PS = Cast<AN1PlayerState>(PC->PlayerState))
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				PS->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}

