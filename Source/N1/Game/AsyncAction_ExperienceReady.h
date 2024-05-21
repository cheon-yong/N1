// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Delegates/Delegate.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class AGameStateBase;
class UN1ExperienceDefinition;

/**
 * 
 */
UCLASS()
class N1_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);
	
	virtual void Activate() override;
	
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	
	void Step3_HandleExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience);
	
	void Step4_BroadcastReady();
	
public:
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	TWeakObjectPtr<UWorld> WorldPtr;
	
};
