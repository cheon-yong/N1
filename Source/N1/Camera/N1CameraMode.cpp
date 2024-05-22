// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/N1CameraMode.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Camera/N1CameraComponent.h"
#include "Camera/N1PlayerCameraManager.h"

FN1CameraModeView::FN1CameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(N1_CAMERA_DEFAULT_FOV)
{
}

void FN1CameraModeView::Blend(const FN1CameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		*this = Other;
		return;
	}
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);
	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);
	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}


UN1CameraMode::UN1CameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FieldOfView = N1_CAMERA_DEFAULT_FOV;
	ViewPitchMin = N1_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = N1_CAMERA_DEFAULT_PITCH_MAX;
	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = EN1CameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

void UN1CameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	 
	UpdateBlending(DeltaTime);
}

void UN1CameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void UN1CameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case EN1CameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case EN1CameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EN1CameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EN1CameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

FVector UN1CameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);
	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}
	return TargetActor->GetActorLocation();

}

FRotator UN1CameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);
	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}
	return TargetActor->GetActorRotation();
}

UN1CameraComponent* UN1CameraMode::GetN1CameraComponent() const
{
	return CastChecked<UN1CameraComponent>(GetOuter());
}

AActor* UN1CameraMode::GetTargetActor() const
{
	const UN1CameraComponent* N1CameraComponent = GetN1CameraComponent();
	return N1CameraComponent->GetTargetActor();
}

UN1CameraModeStack::UN1CameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UN1CameraMode* UN1CameraModeStack::GetCameraModeInstance(TSubclassOf<UN1CameraMode>& CameraModeClass)
{
	check(CameraModeClass);
	for (UN1CameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}
	UN1CameraMode* NewCameraMode = NewObject<UN1CameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);
	CameraModeInstances.Add(NewCameraMode);
	return NewCameraMode;
}

void UN1CameraModeStack::PushCameraMode(TSubclassOf<UN1CameraMode> CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	UN1CameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);
	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;
	CameraModeStack.Insert(CameraMode, 0);
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UN1CameraModeStack::EvaluateStack(float DeltaTime, FN1CameraModeView& OutCameraModeView)
{
	UpdateStack(DeltaTime);

	BlendStack(OutCameraModeView);
}

void UN1CameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
		return;
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UN1CameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);
		CameraMode->UpdateCameraMode(DeltaTime);
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}
	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}

}

void UN1CameraModeStack::BlendStack(FN1CameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}
	const UN1CameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);
	OutCameraModeView = CameraMode->View;
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);
		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}