// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "N1GameState.generated.h"

// forward declaration
class UN1ExperienceManagerComponent;

/**
 * 
 */
UCLASS()
class N1_API AN1GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AN1GameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

public:
	UPROPERTY()
	TObjectPtr<UN1ExperienceManagerComponent> ExperienceManagerComponent;
};
