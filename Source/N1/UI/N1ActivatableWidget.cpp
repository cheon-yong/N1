// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/N1ActivatableWidget.h"

UN1ActivatableWidget::UN1ActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UN1ActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EN1WidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EN1WidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EN1WidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, GameMouseCaptureMode);
	case EN1WidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
