// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/N1GameState.h"
#include "Game/N1ExperienceManagerComponent.h"

AN1GameState::AN1GameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UN1ExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));

}
