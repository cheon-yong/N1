// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerState.h"
#include "Game/N1ExperienceManagerComponent.h"
#include <Game/N1GameMode.h>

AN1PlayerState::AN1PlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AN1PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UN1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UN1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnN1ExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AN1PlayerState::OnExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience)
{
	if (AN1GameMode* GameMode = GetWorld()->GetAuthGameMode<AN1GameMode>())
	{
		const UN1PawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);
		SetPawnData(NewPawnData);
	}
}

void AN1PlayerState::SetPawnData(const UN1PawnData* InPawnData)
{
	check(InPawnData);
	check(!PawnData);

	PawnData = InPawnData;
}
