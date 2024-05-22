// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "N1AnimInstance.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UN1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UN1AnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeInitializeAnimation() override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

public:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
