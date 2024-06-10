// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1GameMode.h"

#include "Character/N1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "N1GameState.h"
#include "Player/N1PlayerController.h"
#include "Player/N1PlayerState.h"
#include "Character/N1Character.h"
#include "Character/N1PawnData.h"
#include "Character/N1PawnExtensionComponent.h"
#include "Game/N1ExperienceManagerComponent.h"
#include "Game/N1ExperienceDefinition.h"
#include "System/N1AssetManager.h"
#include "N1LogChannels.h"
#include <Kismet/GameplayStatics.h>
#include "UI/N1HUD.h"
#include "Player/N1PlayerSpawningManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameMode)

AN1GameMode::AN1GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AN1GameState::StaticClass();
	PlayerControllerClass = AN1PlayerController::StaticClass();
	PlayerStateClass = AN1PlayerState::StaticClass();
	DefaultPawnClass = AN1Character::StaticClass();
	HUDClass = AN1HUD::StaticClass();
}

void AN1GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void AN1GameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UN1ExperienceDefinition::StaticClass()->GetFName()),
			FName(*ExperienceFromOptions));
	}

	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("N1ExperienceDefinition"),
			FName("BP_N1DefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void AN1GameMode::InitGameState()
{
	Super::InitGameState();

	UN1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UN1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnN1ExperienceLoaded::FDelegate::CreateUObject(this,
	&ThisClass::OnExperienceLoaded));
}

void AN1GameMode::OnExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

APawn* AN1GameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UN1PawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogN1, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogN1, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogN1, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

void AN1GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

bool AN1GameMode::IsExperienceLoaded() const
{
	check(GameState);
	const UN1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UN1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	return ExperienceManagerComponent->IsExperienceLoaded();
}

void AN1GameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid());
	UN1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UN1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

UClass* AN1GameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UN1PawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AN1GameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer);
}

bool AN1GameMode::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		// Bot version of Super::PlayerCanRestart_Implementation
		if ((Controller == nullptr) || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	if (UN1PlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UN1PlayerSpawningManagerComponent>())
	{
		return PlayerSpawningComponent->ControllerCanRestart(Controller);
	}

	return true;
}

const UN1PawnData* AN1GameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const AN1PlayerState* PS = InController->GetPlayerState<AN1PlayerState>())
		{
			if (const UN1PawnData* PawnData = PS->GetPawnData<UN1PawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	UN1ExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UN1ExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const UN1ExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		//return UN1AssetManager::Get().GetDefaultPawnData();
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
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
