// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "N1HUD.generated.h"

/**
 * 
 */
UCLASS()
class N1_API AN1HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AN1HUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
