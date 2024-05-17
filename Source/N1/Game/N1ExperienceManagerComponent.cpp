// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/N1ExperienceManagerComponent.h"
#include "System/N1AssetManager.h"
#include "Game/N1ExperienceDefinition.h"
#include <GameFeaturesSubsystemSettings.h>

UN1ExperienceManagerComponent::UN1ExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UN1ExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnN1ExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UN1ExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UN1AssetManager& AssetManager = UN1AssetManager::Get();
	TSubclassOf<UN1ExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	const UN1ExperienceDefinition* Experience = GetDefault<UN1ExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void UN1ExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == EN1ExperienceLoadState::Unloaded);
	LoadState = EN1ExperienceLoadState::Loading;
	UN1AssetManager& AssetManager = UN1AssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	TArray<FName> BundlesToLoad;
	{
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}
	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this,
		&ThisClass::OnExperienceLoadComplete);

	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void UN1ExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	OnExperienceFullLoadCompleted();
}

void UN1ExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != EN1ExperienceLoadState::Loaded);
	LoadState = EN1ExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UN1ExperienceDefinition* UN1ExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EN1ExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
