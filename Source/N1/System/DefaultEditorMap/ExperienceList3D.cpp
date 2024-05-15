// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DefaultEditorMap/ExperienceList3D.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Engine/AssetManager.h>
#include "N1LogChannels.h"
#include "Game/N1UserFacingExperienceDefinition.h"
#include "System/DefaultEditorMap/TeleportToUserFacingExperience.h"

// Sets default values
AExperienceList3D::AExperienceList3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PortalSpacing = 500.f;
	StartPosition = FVector(1200.f, 1200.f, 50.f);
	StartRotator = FRotator(0.f, 180.f, 0.f);
}

// Called when the game starts or when spawned
void AExperienceList3D::BeginPlay()
{
	Super::BeginPlay();
	
	bool loadExperience = LoadUserFacingExperiences();

	if (loadExperience)
	{
		SpawnTeleport();
	}
}

bool AExperienceList3D::LoadUserFacingExperiences()
{
	UserFacingExperienceList.Empty();

	UserFacingExperienceList.Append(AdditionalExperiencesToShow);

	// TO-DO : String을 그대로 쓰는 것은 아쉽다.
	FPrimaryAssetType TypeToLoad(TEXT("N1UserFacingExperienceDefinition"));
	UKismetSystemLibrary::GetPrimaryAssetIdList(TypeToLoad, OutAssetIdList);

	if (OutAssetIdList.Num() > 0)
	{
		// 잘 모르겠어
		//UAssetManager::Get().LoadPrimaryAssets(OutAssetIdList, OutLoadBundle, FStreamableDelegate::CreateUObject(this, &AExperienceList3D::OnLoaded));

		UAssetManager::Get().GetPrimaryAssetDataList(TypeToLoad, OutAssetDatas);

		if (OutAssetDatas.Num() == 0)
		{
			UE_LOG(LogN1, Error, TEXT("Load Bundle is not Loaded"));
			return false;
		}

		for (FAssetData data : OutAssetDatas)
		{
			auto path = data.ToSoftObjectPath();

			if (auto UFE = Cast<UN1UserFacingExperienceDefinition>(UAssetManager::Get().GetStreamableManager().LoadSynchronous(path)))
			{
				UserFacingExperienceList.Add(UFE);
			}
		}

		if (UserFacingExperienceList.Num() == 0)
		{
			UE_LOG(LogN1, Error, TEXT("Cast Fail"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogN1, Error, TEXT("Primary Asset Not Loaded"));
		return false;
	}

	return true;
}

void AExperienceList3D::SpawnTeleport()
{
	int length = UserFacingExperienceList.Num();

	for (int i = 0; i < length; i++)
	{
		float PosY = ((length * -0.5f) + i + 0.5f) * PortalSpacing;
		FRotator Rotator = GetActorRotation();
		FVector Offset(0.f, PosY, 0.f);

		FTransform SpawnTransform(StartRotator, StartPosition + Offset);
		/*AActor* Teleport = GetWorld()->SpawnActorDeferred<AActor>(TeleportClass, SpawnTransform);
		if (Teleport)
		{
			Teleports.Add(Teleport);
		}*/
		ATeleportToUserFacingExperience* Teleport = GetWorld()->SpawnActorDeferred<ATeleportToUserFacingExperience>(TeleportClass, SpawnTransform);
		if (Teleport)
		{
			Teleports.Add(Teleport);
			Teleport->SetUserFacingExperienceDefinition(UserFacingExperienceList[i]);
		}
	}

	for (const auto& Teleport : Teleports)
	{
		if (Teleport.IsValid())
		{
			Teleport.Get()->FinishSpawning(Teleport.Get()->GetActorTransform());
		}
	}
}
