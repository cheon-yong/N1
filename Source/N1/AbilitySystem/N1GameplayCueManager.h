// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "N1GameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UN1GameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
	
public:
	static UN1GameplayCueManager* Get();

	UN1GameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void RefreshGameplayCuePrimaryAsset();
};
