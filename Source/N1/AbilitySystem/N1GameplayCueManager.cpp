// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/N1GameplayCueManager.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "Engine/AssetManager.h"

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));

UN1GameplayCueManager::UN1GameplayCueManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UN1GameplayCueManager* UN1GameplayCueManager::Get()
{
	return Cast<UN1GameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}


void UN1GameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> CuePaths;
	UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
	if (RuntimeGameplayCueSet)
	{
		RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
	}
	
	FAssetBundleData BundleData;
	BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, CuePaths);
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType,
		UFortAssetManager_GameplayCueRefsName);
	UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);
}
