// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/N1GameplayEffectContext.h"

#include "AbilitySystem/N1AbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameplayEffectContext)

class FArchive;

FN1GameplayEffectContext* FN1GameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FN1GameplayEffectContext::StaticStruct()))
	{
		return (FN1GameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FN1GameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FN1GameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(N1GameplayEffectContext, FGameplayEffectContextNetSerializer);
}
#endif


void FN1GameplayEffectContext::SetAbilitySource(const IN1AbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const IN1AbilitySourceInterface* FN1GameplayEffectContext::GetAbilitySource() const
{
	return Cast<IN1AbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FN1GameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}
