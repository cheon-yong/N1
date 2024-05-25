// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "N1ExperienceActionSet.generated.h"

class UGameFeatureAction;

/**
 * 
 */
UCLASS(BlueprintType)
class N1_API UN1ExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UN1ExperienceActionSet();

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditAnywhere, Category = "Feature Dependencies")
	TArray<FString> GameFeaturesToEnable;
};
