// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerController.h"

AN1PlayerController::AN1PlayerController()
{
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