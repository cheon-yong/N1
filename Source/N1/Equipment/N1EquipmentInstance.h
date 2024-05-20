// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "N1EquipmentInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class N1_API UN1EquipmentInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UN1EquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
