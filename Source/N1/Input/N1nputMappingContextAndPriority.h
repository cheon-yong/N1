// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "N1nputMappingContextAndPriority.generated.h"

class UInputMappingContext;
/**
 * 
 */
USTRUCT()
struct N1_API FInputMappingContextAndPriority
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AssetBundles = "Client,Server"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 Priority = 0;

	/** If true, then this mapping context will be registered with the settings when this game feature action is registered. */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bRegisterWithSettings = true;
};
