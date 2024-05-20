// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "N1AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UN1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UN1AnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
