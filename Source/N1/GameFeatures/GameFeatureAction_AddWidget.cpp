// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_AddWidget.h"

#include <Components/GameFrameworkComponentManager.h>
#include "UI/N1HUD.h"
#include "UIExtensionSystem.h"
#include <CommonUIExtensions.h>

void UGameFeatureAction_AddWidget::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();
	ActiveData.LayoutsAdded.Empty();
	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}

void UGameFeatureAction_AddWidget::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

void UGameFeatureAction_AddWidget::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager =
			UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			TSoftClassPtr<AActor> HUDActorClass = AN1HUD::StaticClass();
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this,
					&ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddWidget::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	AN1HUD* HUD = CastChecked<AN1HUD>(Actor);
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		for (const FN1HUDLayoutRequest& Entry : Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
			{
				ActiveData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID,
					ConcreteWidgetClass));
			}
		}
		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FN1HUDElementEntry& Entry : Widgets)
		{
			ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		}
	}
}

void UGameFeatureAction_AddWidget::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	AN1HUD* HUD = CastChecked<AN1HUD>(Actor);
	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActiveData.LayoutsAdded)
	{
		if (AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		}
	}

	ActiveData.LayoutsAdded.Reset();
	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}

void UGameFeatureAction_AddWidget::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
}
