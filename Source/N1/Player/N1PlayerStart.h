// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "GameplayTagContainer.h"

#include "N1PlayerStart.generated.h"


class AController;
class UObject;

enum class EN1PlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};

/**
 * 
 */
UCLASS(Config = Game)
class N1_API AN1PlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	AN1PlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const FGameplayTagContainer& GetGameplayTags() { return StartPointTags; }

	EN1PlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;

	/** Did this player start get claimed by a controller already? */
	bool IsClaimed() const;

	/** If this PlayerStart was not claimed, claim it for ClaimingController */
	bool TryClaim(AController* OccupyingController);

protected:
	/** Check if this PlayerStart is still claimed */
	void CheckUnclaimed();

	/** The controller that claimed this PlayerStart */
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	/** Interval in which we'll check if this player start is not colliding with anyone anymore */
	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claiming")
	float ExpirationCheckInterval = 1.f;

	/** Tags to identify this player start */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer StartPointTags;

	/** Handle to track expiration recurring timer */
	FTimerHandle ExpirationTimerHandle;
	
};
