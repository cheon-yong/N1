// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerController.h"
#include "Player/N1PlayerState.h"
#include "Camera/N1PlayerCameraManager.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "N1GameplayTags.h"

AN1PlayerController::AN1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AN1PlayerCameraManager::StaticClass();
}

void AN1PlayerController::SetIsAutoRunning(const bool bEnabled)
{
	const bool bIsAutoRunning = GetIsAutoRunning();
	if (bEnabled != bIsAutoRunning)
	{
		if (!bEnabled)
		{
			OnEndAutoRun();
		}
		else
		{
			OnStartAutoRun();
		}
	}
}

bool AN1PlayerController::GetIsAutoRunning() const
{
	bool bIsAutoRunning = false;
	if (const UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		bIsAutoRunning = N1ASC->GetTagCount(N1GameplayTags::Status_AutoRunning) > 0;
	}
	return bIsAutoRunning;
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

void AN1PlayerController::OnStartAutoRun()
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		N1ASC->SetLooseGameplayTagCount(N1GameplayTags::Status_AutoRunning, 1);
		K2_OnStartAutoRun();
	}
}

void AN1PlayerController::OnEndAutoRun()
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		N1ASC->SetLooseGameplayTagCount(N1GameplayTags::Status_AutoRunning, 0);
		K2_OnEndAutoRun();
	}
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
