// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "N1PlayerCameraManager.generated.h"


#define N1_CAMERA_DEFAULT_FOV (80.0f)
#define N1_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define N1_CAMERA_DEFAULT_PITCH_MAX (89.0f)

/**
 * 
 */
UCLASS()
class N1_API AN1PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AN1PlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
