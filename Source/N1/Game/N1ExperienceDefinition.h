// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "N1ExperienceDefinition.generated.h"


class UGameFeatureAction;
class UN1PawnData;
class UN1ExperienceActionSet;
//class ULyraExperienceActionSet;

/**
 * 
 */
UCLASS(BlueprintType, Const)
class N1_API UN1ExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UN1ExperienceDefinition();

	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
//	//~End of UObject interface
//
//	//~UPrimaryDataAsset interface
//#if WITH_EDITORONLY_DATA
//	virtual void UpdateAssetBundleData() override;
//#endif
//	//~End of UPrimaryDataAsset interface



public:
	// List of Game Feature Plugins this experience wants to have active
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	// TODO
	///** The default pawn class to spawn for players */
	////@TODO: Make soft?
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<const UN1PawnData> DefaultPawnData;

	// List of actions to perform as this experience is loaded/activated/deactivated/unloaded
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	// List of additional action sets to compose into this experience
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Gameplay)
	TArray<TObjectPtr<UN1ExperienceActionSet>> ActionSets;
};
