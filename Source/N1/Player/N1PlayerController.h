// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "N1PlayerController.generated.h"

class AN1PlayerState;
class UN1AbilitySystemComponent;
/**
 * 
 */
UCLASS()
class N1_API AN1PlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
	
public:
	AN1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "N1|Character")
	void SetIsAutoRunning(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "N1|Character")
	bool GetIsAutoRunning() const;


protected:
	virtual void PostInitializeComponents() override;

	virtual void PostNetInit() override;

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	void OnStartAutoRun();

	void OnEndAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEndAutoRun"))
	void K2_OnEndAutoRun();

	


	AN1PlayerState* GetN1PlayerState() const;

	UN1AbilitySystemComponent* GetN1AbilitySystemComponent() const;

public:

protected:
	bool bHideViewTargetPawnNextFrame = false;
};
