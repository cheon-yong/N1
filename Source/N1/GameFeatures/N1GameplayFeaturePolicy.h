// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"

#include "N1GameplayFeaturePolicy.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, Config = Game)
class UN1GameplayFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
	
public:
	N1_API static UN1GameplayFeaturePolicy& Get();

	UN1GameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer);

	//~UGameFeaturesProjectPolicies interface
	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;
	virtual TArray<FPrimaryAssetId> GetPreloadAssetListForGameFeature(const UGameFeatureData* GameFeatureToLoad, bool bIncludeLoadedAssets = false) const override;
	virtual bool IsPluginAllowed(const FString& PluginURL) const override;
	virtual const TArray<FName> GetPreloadBundleStateForGameFeature() const override;
	virtual void GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const override;
	//~End of UGameFeaturesProjectPolicies interface

private:
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