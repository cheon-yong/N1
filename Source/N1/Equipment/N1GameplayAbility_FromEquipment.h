// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/N1GameplayAbility.h"
#include "N1GameplayAbility_FromEquipment.generated.h"

class UN1EquipmentInstance;

/**
 * 
 */
UCLASS()
class N1_API UN1GameplayAbility_FromEquipment : public UN1GameplayAbility
{
	GENERATED_BODY()
	
public:
	UN1GameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
