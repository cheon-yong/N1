// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Equipment/N1EquipmentInstance.h"
#include "Cosmetics/N1CosmeticAnimationTypes.h"
#include "N1WeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UN1WeaponInstance : public UN1EquipmentInstance
{
	GENERATED_BODY()
	
public:
	UN1WeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer&CosmeticTags) const;
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FN1AnimLayerSelectionSet EquippedAnimSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FN1AnimLayerSelectionSet UnequippedAnimSet;
};
