// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "N1PlayerState.generated.h"

// forward declaration
class UN1ExperienceDefinition;
class UN1PawnData;

/**
 * 
 */
UCLASS()
class N1_API AN1PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AN1PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override final;

	void OnExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience);

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UN1PawnData* InPawnData);


public:
	UPROPERTY()
	TObjectPtr<const UN1PawnData> PawnData;
};
