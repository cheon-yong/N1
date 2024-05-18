// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
class UN1CameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	UN1CameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	UN1CameraComponent* GetN1CameraComponent() const;
	AActor* GetTargetActor() const;


public:
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	float BlendAlpha;

	float BlendWeight;

	FN1CameraModeView View;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	EN1CameraModeBlendFunction BlendFunction;
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
	UN1CameraMode* GetCameraModeInstance(TSubclassOf<UN1CameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<UN1CameraMode> CameraModeClass);
	void EvaluateStack(float DeltaTime, FN1CameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FN1CameraModeView& OutCameraModeView) const;

public:

	UPROPERTY()
	TArray<TObjectPtr<UN1CameraMode>> CameraModeInstances;
	UPROPERTY()
	TArray<TObjectPtr<UN1CameraMode>> CameraModeStack;
};
