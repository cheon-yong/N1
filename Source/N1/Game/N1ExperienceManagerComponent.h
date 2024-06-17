// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "N1ExperienceManagerComponent.generated.h"

namespace UE::GameFeatures { struct FResult; }

// forward declaration
class UN1ExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnN1ExperienceLoaded, const UN1ExperienceDefinition*);

/**
 * 
 */

enum class EN1ExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};


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

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	// Tries to set the current experience, either a UI or gameplay one
	void SetCurrentExperience(FPrimaryAssetId ExperienceId);

	void CallOrRegister_OnExperienceLoaded_HighPriority(FOnN1ExperienceLoaded::FDelegate&& Delegate);

	void CallOrRegister_OnExperienceLoaded(FOnN1ExperienceLoaded::FDelegate&& Delegate);

	void CallOrRegister_OnExperienceLoaded_LowPriority(FOnN1ExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);

	const UN1ExperienceDefinition* GetCurrentExperienceChecked() const;

private:
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();


private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentExperience)
	TObjectPtr<const UN1ExperienceDefinition> CurrentExperience;

	EN1ExperienceLoadState LoadState = EN1ExperienceLoadState::Unloaded;

	FOnN1ExperienceLoaded OnExperienceLoaded_HighPriority;

	FOnN1ExperienceLoaded OnExperienceLoaded;

	FOnN1ExperienceLoaded OnExperienceLoaded_LowPriority;

	int32 NumGameFeaturePluginsLoading = 0;

	int32 NumObservedPausers = 0;

	int32 NumExpectedPausers = 0;

	TArray<FString> GameFeaturePluginURLs;
};
