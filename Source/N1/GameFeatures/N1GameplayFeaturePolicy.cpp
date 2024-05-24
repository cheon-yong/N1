// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/N1GameplayFeaturePolicy.h"
#include "GameFeatureAction_AddGameplayCue.h"
#include "AbilitySystem/N1GameplayCueManager.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"
#include "GameFeaturesSubsystem.h"

UN1GameplayFeaturePolicy::UN1GameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1GameplayFeaturePolicy::InitGameFeatureManager()
{
	Observers.Add(NewObject<UN1GameFeature_AddGameplayCuePaths>());
	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.AddObserver(Observer);
	}

	Super::InitGameFeatureManager();
}

void UN1GameplayFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();

	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.RemoveObserver(Observer);
	}
	Observers.Empty();
}

void UN1GameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCue* AddGameplayCueGFA =
			Cast<UGameFeatureAction_AddGameplayCue>(Action))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;
			if (UN1GameplayCueManager* GCM = UN1GameplayCueManager::Get())
			{
				UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
				const int32 PreInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
					GCM->AddGameplayCueNotifyPath(MutablePath, /*bShouldRescanCueAssets=*/false);
				}
				if (!DirsToAdd.IsEmpty())
				{
					GCM->InitializeRuntimeObjectLibrary();
				}
				const int32 PostInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
				if (PreInitializeNumCues != PostInitializeNumCues)
				{
					GCM->RefreshGameplayCuePrimaryAsset();
				}
			}
		}
	}
}

void UN1GameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		if (const UGameFeatureAction_AddGameplayCue* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCue>(GameFeatureData))
		{
			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->DirectoryPathsToAdd;
			if (UN1GameplayCueManager* GCM = UN1GameplayCueManager::Get())
			{
				int32 NumRemoved = 0;
				for (const FDirectoryPath& Directory : DirsToAdd)
				{
					FString MutablePath = Directory.Path;
					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
					NumRemoved += GCM->RemoveGameplayCueNotifyPath(MutablePath, /** bShouldRescanCueAssets=*/ false);
				}
				ensure(NumRemoved == DirsToAdd.Num());
				if (NumRemoved > 0)
				{
					GCM->InitializeRuntimeObjectLibrary();
				}
			}
		}
	}
}
