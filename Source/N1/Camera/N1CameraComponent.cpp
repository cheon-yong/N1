// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/N1CameraComponent.h"
#include "Camera/N1CameraMode.h"

UN1CameraComponent::UN1CameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1CameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
		CameraModeStack = NewObject<UN1CameraModeStack>(this);
}

void UN1CameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();
}

void UN1CameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (const TSubclassOf<UN1CameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			// CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
