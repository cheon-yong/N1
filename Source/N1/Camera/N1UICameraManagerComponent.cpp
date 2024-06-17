// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1UICameraManagerComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "N1PlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1UICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

UN1UICameraManagerComponent* UN1UICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (AN1PlayerCameraManager* PCCamera = Cast<AN1PlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UN1UICameraManagerComponent::UN1UICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UN1UICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UN1UICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<AN1PlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UN1UICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UN1UICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void UN1UICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
