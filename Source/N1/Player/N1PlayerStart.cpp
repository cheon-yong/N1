// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerStart.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"

AN1PlayerStart::AN1PlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EN1PlayerStartLocationOccupancy AN1PlayerStart::GetLocationOccupancy(AController* const ControllerPawnToFit) const
{
	UWorld* const World = GetWorld();
	if (HasAuthority() && World)
	{
		if (AGameModeBase* AuthGameMode = World->GetAuthGameMode())
		{
			TSubclassOf<APawn> PawnClass = AuthGameMode->GetDefaultPawnClassForController(ControllerPawnToFit);
			const APawn* const PawnToFit = PawnClass ? GetDefault<APawn>(PawnClass) : nullptr;

			FVector ActorLocation = GetActorLocation();
			const FRotator ActorRotation = GetActorRotation();

			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation, nullptr))
			{
				return EN1PlayerStartLocationOccupancy::Empty;
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				return EN1PlayerStartLocationOccupancy::Partial;
			}
		}
	}

	return EN1PlayerStartLocationOccupancy::Full;
}

bool AN1PlayerStart::IsClaimed() const
{
	return ClaimingController != nullptr;
}

bool AN1PlayerStart::TryClaim(AController* OccupyingController)
{
	if (OccupyingController != nullptr && !IsClaimed())
	{
		ClaimingController = OccupyingController;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ExpirationTimerHandle, FTimerDelegate::CreateUObject(this, &AN1PlayerStart::CheckUnclaimed), ExpirationCheckInterval, true);
		}
		return true;
	}
	return false;
}	

void AN1PlayerStart::CheckUnclaimed()
{
	if (ClaimingController != nullptr && ClaimingController->GetPawn() != nullptr && GetLocationOccupancy(ClaimingController) == EN1PlayerStartLocationOccupancy::Empty)
	{
		ClaimingController = nullptr;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ExpirationTimerHandle);
		}
	}
}
