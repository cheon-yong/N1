// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "N1ExperienceManagerComponent.generated.h"

// forward declaration
class UN1ExperienceDefinition;

/**
 * 
 */

enum class EN1ExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnN1ExperienceLoaded, const UN1ExperienceDefinition*);


UCLASS()
class N1_API UN1ExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	UN1ExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool IsExperienceLoaded() const {
		return (LoadState == EN1ExperienceLoadState::Loaded) && (CurrentExperience !=
			nullptr);
	}

	void CallOrRegister_OnExperienceLoaded_HighPriority(FOnN1ExperienceLoaded::FDelegate&& Delegate);

	void CallOrRegister_OnExperienceLoaded(FOnN1ExperienceLoaded::FDelegate&& Delegate);

	void CallOrRegister_OnExperienceLoaded_LowPriority(FOnN1ExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);

	void StartExperienceLoad();

	void OnExperienceLoadComplete();

	void OnExperienceFullLoadCompleted();

	const UN1ExperienceDefinition* GetCurrentExperienceChecked() const;

	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);

private:
	UPROPERTY()
	TObjectPtr<const UN1ExperienceDefinition> CurrentExperience;

	EN1ExperienceLoadState LoadState = EN1ExperienceLoadState::Unloaded;

	FOnN1ExperienceLoaded OnExperienceLoaded_HighPriority;

	FOnN1ExperienceLoaded OnExperienceLoaded;

	FOnN1ExperienceLoaded OnExperienceLoaded_LowPriority;

	int32 NumGameFeaturePluginsLoading = 0;

	TArray<FString> GameFeaturePluginURLs;
};
