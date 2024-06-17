// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/PlayerCameraManager.h"

#include "N1PlayerCameraManager.generated.h"

class FDebugDisplayInfo;
class UCanvas;
class UObject;


#define N1_CAMERA_DEFAULT_FOV			(80.0f)
#define N1_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define N1_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UN1UICameraManagerComponent;

/**
 * AN1PlayerCameraManager
 *
 *	The base player camera manager class used by this project.
 */
UCLASS(notplaceable, MinimalAPI)
class AN1PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	public:

	AN1PlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	UN1UICameraManagerComponent* GetUICameraComponent() const;

protected:

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

private:
	/** The UI Camera Component, controls the camera when UI is doing something important that gameplay doesn't get priority over. */
	UPROPERTY(Transient)
	TObjectPtr<UN1UICameraManagerComponent> UICamera;
};
