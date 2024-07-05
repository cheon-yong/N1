// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/N1GameplayAbility.h"
#include "Interaction/InteractionOption.h"

#include "N1GameplayAbility_Interact.generated.h"

class UIndicatorDescriptor;
class UObject;
class UUserWidget;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;

/**
 * UN1GameplayAbility_Interact
 *
 * Gameplay ability used for character interacting
 */
UCLASS(Abstract)
class UN1GameplayAbility_Interact : public UN1GameplayAbility
{
	GENERATED_BODY()

public:

	UN1GameplayAbility_Interact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions);

	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> CurrentOptions;

	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;

protected:

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRange = 500;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;
};
