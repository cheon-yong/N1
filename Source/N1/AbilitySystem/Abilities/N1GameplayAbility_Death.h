// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "N1GameplayAbility.h"

#include "N1GameplayAbility_Death.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;


/**
 * UN1GameplayAbility_Death
 *
 *	Gameplay ability used for handling death.
 *	Ability is activated automatically via the "GameplayEvent.Death" ability trigger tag.
 */
UCLASS(Abstract)
class UN1GameplayAbility_Death : public UN1GameplayAbility
{
	GENERATED_BODY()

	public:

	UN1GameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Starts the death sequence.
	UFUNCTION(BlueprintCallable, Category = "N1|Ability")
	void StartDeath();

	// Finishes the death sequence.
	UFUNCTION(BlueprintCallable, Category = "N1|Ability")
	void FinishDeath();

protected:

	// If enabled, the ability will automatically call StartDeath.  FinishDeath is always called when the ability ends if the death was started.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|Death")
	bool bAutoStartDeath;
};
