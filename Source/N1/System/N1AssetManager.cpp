// Fill out your copyright notice in the Description page of Project Settings.


#include "System/N1AssetManager.h"

void UN1AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	ShouldLogAssetLoads();
}

UN1AssetManager& UN1AssetManager::Get()
{
	check(GEngine);

	if (UN1AssetManager* Singleton = Cast<UN1AssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal,
		TEXT("invalid AssetManagerClassName in DefaultEngine.ini(project setting);"
			" it must be N1AssetManager"));

	return *NewObject<UN1AssetManager>();
}

bool UN1AssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void UN1AssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}

UObject* UN1AssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		return AssetPath.TryLoad();
	}

	return nullptr;
}
