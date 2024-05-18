// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/N1CameraMode.h"
#include "N1CameraMode_ThirdPerson.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class N1_API UN1CameraMode_ThirdPerson : public UN1CameraMode
{
	GENERATED_BODY()
	
public:
	UN1CameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
