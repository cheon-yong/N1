// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/N1WeaponInstance.h"
#include "Cosmetics/N1CharacterPartTypes.h"

UN1WeaponInstance::UN1WeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSubclassOf<UAnimInstance> UN1WeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FN1AnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
