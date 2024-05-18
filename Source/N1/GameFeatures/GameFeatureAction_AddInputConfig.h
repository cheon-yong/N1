// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/GameFeatureAction_WorldAction.h"
#include <Input/N1MappableConfigPair.h>

#include "GameFeatureAction_AddInputConfig.generated.h"


struct FComponentRequestHandle;
class APawn;

/**
 * 
 */
UCLASS()
class N1_API UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldAction
{
	GENERATED_BODY()
	
public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext&
		ChangeContext) override;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext
		ChangeContext);
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void Reset(FPerContextData& ActiveData);

private:
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	UPROPERTY(EditAnywhere)
	TArray<FN1MappableConfigPair> InputConfigs;
};
