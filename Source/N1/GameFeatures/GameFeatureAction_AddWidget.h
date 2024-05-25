// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIExtensionSystem.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "GameFeatureAction_WorldAction.h"
#include "GameFeatureAction_AddWidget.generated.h"

struct FComponentRequestHandle;
struct FUIExtensionHandle;

USTRUCT()
struct FN1HUDLayoutRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	UPROPERTY(EditAnywhere, Category = UI, meta=(Categories="UI"))
	FGameplayTag LayerID;
};

USTRUCT()
struct FN1HUDElementEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = UI, meta = (Categories = "UI"))
	FGameplayTag SlotID;
};

/**
 * 
 */
UCLASS(meta = (DisplayName = "Add Widgets"))
class N1_API UGameFeatureAction_AddWidget : public UGameFeatureAction_WorldAction
{
	GENERATED_BODY()
	
public:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;

		TArray<FUIExtensionHandle> ExtensionHandles;
	};

public:
	void Reset(FPerContextData& ActiveData);

	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);

	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);

	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

public:
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FN1HUDLayoutRequest> Layout;

	UPROPERTY(EditAnywhere, Category = UI)
	TArray<FN1HUDElementEntry> Widgets;
};
