// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/N1ExperienceActionSet.h"
#include "GameFeatureAction.h"

UN1ExperienceActionSet::UN1ExperienceActionSet() : Super()
{
}

#if WITH_EDITORONLY_DATA
void UN1ExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();
	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif