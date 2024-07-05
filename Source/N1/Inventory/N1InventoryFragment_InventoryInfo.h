// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/N1InventoryItemDefinition.h"
#include "Styling/SlateBrush.h"

#include "N1InventoryFragment_InventoryInfo.generated.h"

class UObject;

UCLASS()
class UN1InventoryFragment_InventoryInfo : public UN1InventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText Name;
};
