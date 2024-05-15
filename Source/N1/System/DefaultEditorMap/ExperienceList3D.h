// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExperienceList3D.generated.h"

// forward declaration
class UN1UserFacingExperienceDefinition;
class ATeleportToUserFacingExperience;

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

// variable
public:	
	// About Only Visible
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPrimaryAssetId> OutAssetIdList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAssetData> OutAssetDatas;

	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AActor>> Teleports;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ExperiencePortals;

	// About Experience
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	TArray<UN1UserFacingExperienceDefinition*> AdditionalExperiencesToShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	TArray<UN1UserFacingExperienceDefinition*> UserFacingExperienceList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	TSubclassOf<ATeleportToUserFacingExperience> TeleportClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	bool bShowAllExperiences = true;

	// About Position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	float PortalSpacing;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Position")
	FVector StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	FRotator StartRotator;

};
