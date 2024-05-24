// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"

#include "N1GameplayFeaturePolicy.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UN1GameplayFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
	
public:
	UN1GameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};

UCLASS()
class UN1GameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()
	
public:
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString&
		PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString&
		PluginName, const FString& PluginURL) override;
};