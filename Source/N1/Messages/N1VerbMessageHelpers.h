// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "N1VerbMessageHelpers.generated.h"


struct FGameplayCueParameters;
struct FN1VerbMessage;

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;


/**
 * 
 */
UCLASS()
class N1_API UN1VerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "N1")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "N1")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "N1")
	static FGameplayCueParameters VerbMessageToCueParameters(const FN1VerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "N1")
	static FN1VerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
