// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerState.h"
#include "Game/N1ExperienceManagerComponent.h"
#include <Game/N1GameMode.h>
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include <AbilitySystem/N1AbilitySet.h>
#include "Character/N1PawnData.h"

AN1PlayerState::AN1PlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UN1AbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}

void AN1PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	{
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

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
	for (UN1AbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}
