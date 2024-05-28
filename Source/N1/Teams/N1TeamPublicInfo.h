// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "N1TeamInfoBase.h"

#include "N1TeamPublicInfo.generated.h"

class UN1TeamCreationComponent;
class UN1TeamDisplayAsset;
class UObject;
struct FFrame;

/**
 * 
 */
UCLASS()
class N1_API AN1TeamPublicInfo : public AN1TeamInfoBase
{
	GENERATED_BODY()
	
	friend UN1TeamCreationComponent;

public:
	AN1TeamPublicInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UN1TeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UN1TeamDisplayAsset> NewDisplayAsset);

private:
	UPROPERTY(ReplicatedUsing = OnRep_TeamDisplayAsset)
	TObjectPtr<UN1TeamDisplayAsset> TeamDisplayAsset;
};
