// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ControllerComponent.h"

#include "N1IndicatorManagerComponent.generated.h"

class AController;
class UIndicatorDescriptor;
class UObject;
struct FFrame;

/**
 * @class UN1IndicatorManagerComponent
 */
UCLASS(BlueprintType, Blueprintable)
class N1_API UN1IndicatorManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UN1IndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

	static UN1IndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = Indicator)
	void AddIndicator(UIndicatorDescriptor* IndicatorDescriptor);
	
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor);

	DECLARE_EVENT_OneParam(UN1IndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Descriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

	const TArray<UIndicatorDescriptor*>& GetIndicators() const { return Indicators; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
};
