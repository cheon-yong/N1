// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "N1PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class N1_API AN1PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AN1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
