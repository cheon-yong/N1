// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "N1MappableConfigPair.generated.h"

class UPlayerMappableInputConfig;
/**
 * 
 */
USTRUCT(BlueprintType)
struct N1_API FN1MappableConfigPair
{
	GENERATED_BODY()
	
	FN1MappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<const UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};
