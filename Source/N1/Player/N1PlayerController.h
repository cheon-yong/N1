// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "N1PlayerController.generated.h"

class AN1PlayerState;
class UN1AbilitySystemComponent;
/**
 * 
 */
UCLASS()
class N1_API AN1PlayerController : public AModularPlayerController
{
	GENERATED_BODY()
	
public:
	AN1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void PostInitializeComponents() override;

	virtual void PostNetInit() override;

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	AN1PlayerState* GetN1PlayerState() const;

	UN1AbilitySystemComponent* GetN1AbilitySystemComponent() const;

};
