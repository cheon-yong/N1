// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerController.h"
#include "Player/N1PlayerState.h"
#include "Camera/N1PlayerCameraManager.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"

AN1PlayerController::AN1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AN1PlayerCameraManager::StaticClass();
}

void AN1PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AN1PlayerController::PostNetInit()
{
	Super::PostNetInit();
}

void AN1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AN1PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AN1PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UN1AbilitySystemComponent* ASC = GetN1AbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

AN1PlayerState* AN1PlayerController::GetN1PlayerState() const
{
	return CastChecked<AN1PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UN1AbilitySystemComponent* AN1PlayerController::GetN1AbilitySystemComponent() const
{
	const AN1PlayerState* PS = GetN1PlayerState();
	return (PS ? PS->GetN1AbilitySystemComponent() : nullptr);
}
