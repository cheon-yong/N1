// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "N1ActivatableWidget.h"

#include "N1HUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;


/**
 * UN1HUDLayout
 *
 *	Widget used to lay out the player's HUD (typically specified by an Add Widgets action in the experience)
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "N1 HUD Layout", Category = "N1|HUD"))
class UN1HUDLayout : public UN1ActivatableWidget
{
	GENERATED_BODY()

public:

	UN1HUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
