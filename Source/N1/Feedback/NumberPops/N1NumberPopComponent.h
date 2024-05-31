// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ControllerComponent.h"
#include "GameplayTagContainer.h"

#include "N1NumberPopComponent.generated.h"

class UObject;
struct FFrame;

USTRUCT(BlueprintType)
struct FN1NumberPopRequest
{
	GENERATED_BODY()

	// The world location to create the number pop at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N1|Number Pops")
	FVector WorldLocation;

	// Tags related to the source/cause of the number pop (for determining a style)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N1|Number Pops")
	FGameplayTagContainer SourceTags;

	// Tags related to the target of the number pop (for determining a style)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N1|Number Pops")
	FGameplayTagContainer TargetTags;

	// The number to display
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N1|Number Pops")
	int32 NumberToDisplay = 0;

	// Whether the number is 'critical' or not (@TODO: move to a tag)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N1|Number Pops")
	bool bIsCriticalDamage = false;

	FN1NumberPopRequest()
		: WorldLocation(ForceInitToZero)
	{
	}
};

/**
 * 
 */
UCLASS(Abstract)
class N1_API UN1NumberPopComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public :
	UN1NumberPopComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Adds a damage number to the damage number list for visualization */
	UFUNCTION(BlueprintCallable, Category = Foo)
	virtual void AddNumberPop(const FN1NumberPopRequest& NewRequest) {}
};
