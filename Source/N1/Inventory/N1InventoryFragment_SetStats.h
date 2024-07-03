// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Inventory/N1InventoryItemDefinition.h"

#include "N1InventoryFragment_SetStats.generated.h"

class UN1InventoryItemInstance;
class UObject;
struct FGameplayTag;

UCLASS()
class UN1InventoryFragment_SetStats : public UN1InventoryItemFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(UN1InventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
