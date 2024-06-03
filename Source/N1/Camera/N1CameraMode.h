// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"

#include "N1CameraMode.generated.h"

UENUM(BlueprintType)
enum class EN1CameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

struct FN1CameraModeView
{
public:
	FN1CameraModeView();
	void Blend(const FN1CameraModeView& Other, float OtherWeight);

public:
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};


class UN1CameraComponent;

UCLASS(Abstract, NotBlueprintable)
class N1_API UN1CameraMode : public UObject
{
	GENERATED_BODY()
	
public:

	UN1CameraMode();

	UN1CameraComponent* GetN1CameraComponent() const;

	virtual UWorld* GetWorld() const override;

	AActor* GetTargetActor() const;

	const FN1CameraModeView& GetCameraModeView() const { return View; }

	// Called when this camera mode is activated on the camera mode stack.
	virtual void OnActivation() {};

	// Called when this camera mode is deactivated on the camera mode stack.
	virtual void OnDeactivation() {};

	void UpdateCameraMode(float DeltaTime);

	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }
	void SetBlendWeight(float Weight);

	FGameplayTag GetCameraTypeTag() const
	{
		return CameraTypeTag;
	}

	virtual void DrawDebug(UCanvas* Canvas) const;

protected:

	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;

	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);


public:

protected:
	// A tag that can be queried by gameplay code that cares when a kind of camera mode is active
	// without having to ask about a specific mode (e.g., when aiming downsights to get more accuracy)
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	FGameplayTag CameraTypeTag;

	// View output produced by the camera mode.
	FN1CameraModeView View;

	// The horizontal field of view (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	// Minimum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	// Maximum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	// How long it takes to blend in this mode.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// Function used for blending.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	EN1CameraModeBlendFunction BlendFunction;

	// Exponent used by blend functions to control the shape of the curve.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// Linear blend alpha used to determine the blend weight.
	float BlendAlpha;

	// Blend weight calculated using the blend alpha and function.
	float BlendWeight;

protected:
	/** If true, skips all interpolation and puts camera in ideal location.  Automatically set to false next frame. */
	UPROPERTY(transient)
	uint32 bResetInterpolation : 1;
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
	

	void ActivateStack();

	void DeactivateStack();

	bool IsStackActivate() const { return bIsActive; }

	void PushCameraMode(TSubclassOf<UN1CameraMode> CameraModeClass);
	bool EvaluateStack(float DeltaTime, FN1CameraModeView& OutCameraModeView);
	
	void DrawDebug(UCanvas* Canvas) const;
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
	UN1CameraMode* GetCameraModeInstance(TSubclassOf<UN1CameraMode>& CameraModeClass);
	void UpdateStack(float DeltaTime);
	void BlendStack(FN1CameraModeView& OutCameraModeView) const;

public:

protected:
	bool bIsActive;

	UPROPERTY()
	TArray<TObjectPtr<UN1CameraMode>> CameraModeInstances;
	UPROPERTY()
	TArray<TObjectPtr<UN1CameraMode>> CameraModeStack;
};
