// Fill out your copyright notice in the Description page of Project Settings.


#include "System/N1GameData.h"
#include "System/N1AssetManager.h"

UN1GameData::UN1GameData()
{
}

const UN1GameData& UN1GameData::Get()
{
	return UN1AssetManager::Get().GetGameData();
}
