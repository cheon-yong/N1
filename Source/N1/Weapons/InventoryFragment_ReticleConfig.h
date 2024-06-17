// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/N1InventoryItemDefinition.h"
#include "InventoryFragment_ReticleConfig.generated.h"

class UN1ReticleWidgetBase;
/**
 * 
 */
UCLASS()
class N1_API UInventoryFragment_ReticleConfig : public UN1InventoryItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
	TArray<TSubclassOf<UN1ReticleWidgetBase>> ReticleWidgets;
};
