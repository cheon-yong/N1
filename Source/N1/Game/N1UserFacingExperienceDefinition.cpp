// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/N1UserFacingExperienceDefinition.h"
#include "CommonSessionSubsystem.h"
#include "Containers/UnrealString.h"
#include "UObject/NameTypes.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
//#include "Replays/LyraReplaySubsystem.h"

UCommonSession_HostSessionRequest* UN1UserFacingExperienceDefinition::CreateHostingRequest(const UObject* WorldContextObject) const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	const FString UserFacingExperienceName = GetPrimaryAssetId().PrimaryAssetName.ToString();

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UCommonSession_HostSessionRequest* Result = nullptr;

	if (UCommonSessionSubsystem* Subsystem = GameInstance ? GameInstance->GetSubsystem<UCommonSessionSubsystem>() : nullptr)
	{
		Result = Subsystem->CreateOnlineHostSessionRequest();
	}

	if (!Result)
	{
		// Couldn't use the subsystem so create one
		Result = NewObject<UCommonSession_HostSessionRequest>();
		Result->OnlineMode = ECommonSessionOnlineMode::Online;
		Result->bUseLobbies = true;
	}
	Result->MapID = MapID;
	Result->ModeNameForAdvertisement = UserFacingExperienceName;
	Result->ExtraArgs = ExtraArgs;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
	Result->MaxPlayerCount = MaxPlayerCount;

	/*if (ULyraReplaySubsystem::DoesPlatformSupportReplays())
	{
		if (bRecordReplay)
		{
			Result->ExtraArgs.Add(TEXT("DemoRec"), FString());
		}
	}*/

	return Result;
}
