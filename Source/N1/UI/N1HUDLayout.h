// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/N1ActivatableWidget.h"
#include "N1HUDLayout.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, meta = (DisplayName = "N1 HUD Layout", Category = "N1|HUD"))
class N1_API UN1HUDLayout : public UN1ActivatableWidget
{
	GENERATED_BODY()
	
public:
	UN1HUDLayout(const FObjectInitializer& ObjectInitializer);

};
