// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1GameMode.h"
#include "Character/N1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

AN1GameMode::AN1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

FTransform AN1GameMode::GetRandomStartTransform() const
{
	if (PlayerStartArray.Num() == 0)
	{
		return FTransform(FVector(0.0f, 0.0f, 230.0f));
	}

	int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);
	return PlayerStartArray[RandIndex]->GetActorTransform();
}

void AN1GameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
	APlayerState* KillerPlayerState = Killer->PlayerState;
	if (KillerPlayerState)
	{
		/*KillerPlayerState->SetScore(KillerPlayerState->GetScore() + 1);

		if (KillerPlayerState->GetScore() > 2)
		{
			FinishMatch();
		}*/
	}
}

void AN1GameMode::StartPlay()
{
	Super::StartPlay();
}

void AN1GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AN1GameMode::DefaultGameTimer()
{
}

void AN1GameMode::FinishMatch()
{
}
