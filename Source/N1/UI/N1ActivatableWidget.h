// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "N1ActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EN1WidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class N1_API UN1ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UN1ActivatableWidget(const FObjectInitializer& ObjectInitializer);

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EN1WidgetInputMode InputConfig = EN1WidgetInputMode::Default;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
