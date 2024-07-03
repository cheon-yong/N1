// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/N1InventoryItemDefinition.h"
#include "Styling/SlateBrush.h"

#include "N1InventoryFragment_QuickBarIcon.generated.h"

class UObject;

UCLASS()
class UN1InventoryFragment_QuickBarIcon : public UN1InventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush AmmoBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayNameWhenEquipped;
};
