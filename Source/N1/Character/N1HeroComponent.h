// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Input/N1MappableConfigPair.h"
#include "GameFeatures/GameFeatureAction_AddContext.h"
#include "N1HeroComponent.generated.h"

class UN1CameraMode;
struct FInputActionValue;
//struct FN1nputMappingContextAndPriority;
/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class N1_API UN1HeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UN1HeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// GameFrameworkInitState
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
		FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag
		CurrentState, FGameplayTag DesiredState) final;
	virtual void CheckDefaultInitialization() final;

	// Camera
	TSubclassOf<UN1CameraMode> DetermineCameraMode() const;

	// Input
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
public:
	/*UPROPERTY(EditAnywhere)
	TArray<FN1MappableConfigPair> DefaultConfigs;*/

	static const FName NAME_BindInputsNow;

	UPROPERTY(EditAnywhere)
	TArray<FInputMappingContextAndPriority> DefaultInputMappings;

};
