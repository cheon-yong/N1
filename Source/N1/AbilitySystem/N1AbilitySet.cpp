// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/N1AbilitySet.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/N1GameplayAbility.h"

UN1AbilitySet::UN1AbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1AbilitySet::GiveToAbilitySystem(UN1AbilitySystemComponent* ASC, FN1AbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(ASC);
	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FN1AbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		UN1GameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UN1GameplayAbility>();
		
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);
		
		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

}

void FN1AbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FN1AbilitySet_GrantedHandles::TakeFromAbilitySystem(UN1AbilitySystemComponent* ASC)
{
	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}
}
