// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/N1PlayerCameraManager.h"

AN1PlayerCameraManager::AN1PlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = N1_CAMERA_DEFAULT_FOV;
	ViewPitchMin = N1_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = N1_CAMERA_DEFAULT_PITCH_MAX;
}
