// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "N1CameraComponent.generated.h"


class UN1CameraModeStack;
class UN1CameraMode;

template <class TClass> class TSubclassOf;
DECLARE_DELEGATE_RetVal(TSubclassOf<UN1CameraMode>, FN1CameraModeDelegate);

/**
 * 
 */
UCLASS()
class N1_API UN1CameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UN1CameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	void UpdateCameraModes();
	AActor* GetTargetActor() const { return GetOwner(); }

	UFUNCTION(BlueprintPure, Category = "N1|Camera")
	static UN1CameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UN1CameraComponent>() : nullptr); }
public:
	UPROPERTY()
	TObjectPtr<UN1CameraModeStack> CameraModeStack;
	FN1CameraModeDelegate DetermineCameraModeDelegate;
	
};
