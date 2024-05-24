// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddGameplayCue.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UGameFeatureAction_AddGameplayCue : public UGameFeatureAction
{
	GENERATED_BODY()
	
public:
	UGameFeatureAction_AddGameplayCue();

	UPROPERTY(EditAnywhere, Category = "GameFeature|GameplayCues")
	TArray<FDirectoryPath> DirectoryPathsToAdd;
};
