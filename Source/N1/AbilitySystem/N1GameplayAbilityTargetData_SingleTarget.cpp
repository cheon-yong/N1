// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1GameplayAbilityTargetData_SingleTarget.h"

#include "N1GameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameplayAbilityTargetData_SingleTarget)

struct FGameplayEffectContextHandle;

//////////////////////////////////////////////////////////////////////

void FN1GameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);

	// Add game-specific data
	if (FN1GameplayEffectContext* TypedContext = FN1GameplayEffectContext::ExtractEffectContext(Context))
	{
		TypedContext->CartridgeID = CartridgeID;
	}
}

bool FN1GameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}

