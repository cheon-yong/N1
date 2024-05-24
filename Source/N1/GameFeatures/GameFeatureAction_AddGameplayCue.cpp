// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_AddGameplayCue.h"

UGameFeatureAction_AddGameplayCue::UGameFeatureAction_AddGameplayCue()
	: Super()
{
	DirectoryPathsToAdd.Add(FDirectoryPath{ TEXT("/GameplayCues") });
}
