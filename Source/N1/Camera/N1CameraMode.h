// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "N1CameraMode.generated.h"

UCLASS(Abstract, NotBlueprintable)
class UN1CameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	UN1CameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

/**
 * 
 */
UCLASS()
class UN1CameraModeStack : public UObject
{
	GENERATED_BODY()
	public:
		UN1CameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UPROPERTY()
	TArray<TObjectPtr<UN1CameraMode>> CameraModeInstances;
	UPROPERTY()
	TArray<TObjectPtr<UN1CameraMode>> CameraModeStack;
};
