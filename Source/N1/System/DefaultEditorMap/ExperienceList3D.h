// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExperienceList3D.generated.h"

// forward declaration
class UN1UserFacingExperienceDefinition;

UCLASS()
class N1_API AExperienceList3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExperienceList3D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool LoadUserFacingExperiences();

	void SpawnTeleport();

	void OnLoaded();

// variable
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UN1UserFacingExperienceDefinition*> UserFacingExperienceList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPrimaryAssetId> OutAssetIdList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> OutLoadBundle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAssetData> OutAssetDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowAllExperiences = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ExperiencePortals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PortalSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> TeleportClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UN1UserFacingExperienceDefinition*> AdditionalExperiencesToShow;

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<AActor>> Teleports;
};
