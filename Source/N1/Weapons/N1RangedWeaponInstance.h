// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/N1WeaponInstance.h"
#include "N1RangedWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UN1RangedWeaponInstance : public UN1WeaponInstance
{
	GENERATED_BODY()
	
public:
	UN1RangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;
};
