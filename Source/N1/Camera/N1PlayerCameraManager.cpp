// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1PlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "N1CameraComponent.h"
#include "N1UICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1PlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

AN1PlayerCameraManager::AN1PlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = N1_CAMERA_DEFAULT_FOV;
	ViewPitchMin = N1_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = N1_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UN1UICameraManagerComponent>(UICameraComponentName);
}

UN1UICameraManagerComponent* AN1PlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AN1PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AN1PlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("N1PlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UN1CameraComponent* CameraComponent = UN1CameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}

