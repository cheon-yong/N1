// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerSpawningManagerComponent.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "Player/N1PlayerStart.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1PlayerSpawningManagerComponent)

DEFINE_LOG_CATEGORY_STATIC(LogPlayerSpawning, Log, All);

UN1PlayerSpawningManagerComponent::UN1PlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UN1PlayerSpawningManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &ThisClass::OnLevelAdded);

	UWorld* World = GetWorld();
	World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::HandleOnActorSpawned));

	for (TActorIterator<AN1PlayerStart> It(World); It; ++It)
	{
		if (AN1PlayerStart* PlayerStart = *It)
		{
			CachedPlayerStarts.Add(PlayerStart);
		}
	}
}

void UN1PlayerSpawningManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

APlayerStart* UN1PlayerSpawningManagerComponent::GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<AN1PlayerStart*>& FoundStartPoints) const
{
	if (Controller)
	{
		TArray<AN1PlayerStart*> UnOccupiedStartPoints;
		TArray<AN1PlayerStart*> OccupiedStartPoints;

		for (AN1PlayerStart* StartPoint : FoundStartPoints)
		{
			EN1PlayerStartLocationOccupancy State = StartPoint->GetLocationOccupancy(Controller);

			switch (State)
			{
			case EN1PlayerStartLocationOccupancy::Empty:
				UnOccupiedStartPoints.Add(StartPoint);
				break;
			case EN1PlayerStartLocationOccupancy::Partial:
				OccupiedStartPoints.Add(StartPoint);
				break;

			}
		}

		if (UnOccupiedStartPoints.Num() > 0)
		{
			return UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			return OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}

	return nullptr;
}

AActor* UN1PlayerSpawningManagerComponent::ChoosePlayerStart(AController* Player)
{
	if (Player)
	{
#if WITH_EDITOR
		if (APlayerStart* PlayerStart = FindPlayFromHereStart(Player))
		{
			return PlayerStart;
		}
#endif

		TArray<AN1PlayerStart*> StarterPoints;
		for (auto StartIt = CachedPlayerStarts.CreateIterator(); StartIt; ++StartIt)
		{
			if (AN1PlayerStart* Start = (*StartIt).Get())
			{
				StarterPoints.Add(Start);
			}
			else
			{
				StartIt.RemoveCurrent();
			}
		}

		if (APlayerState* PlayerState = Player->GetPlayerState<APlayerState>())
		{
			// start dedicated spectators at any random starting location, but they do not claim it
			if (PlayerState->IsOnlyASpectator())
			{
				if (!StarterPoints.IsEmpty())
				{
					return StarterPoints[FMath::RandRange(0, StarterPoints.Num() - 1)];
				}

				return nullptr;
			}
		}

		AActor* PlayerStart = OnChoosePlayerStart(Player, StarterPoints);

		if (!PlayerStart)
		{
			PlayerStart = GetFirstRandomUnoccupiedPlayerStart(Player, StarterPoints);
		}

		if (AN1PlayerStart* N1Start = Cast<AN1PlayerStart>(PlayerStart))
		{
			N1Start->TryClaim(Player);
		}

		return PlayerStart;
	}

	return nullptr;
}

bool UN1PlayerSpawningManagerComponent::ControllerCanRestart(AController* Player)
{
	bool bCanRestart = true;

	// TODO Can they restart?

	return bCanRestart;
}

void UN1PlayerSpawningManagerComponent::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	OnFinishRestartPlayer(NewPlayer, StartRotation);
	K2_OnFinishRestartPlayer(NewPlayer, StartRotation);
}

void UN1PlayerSpawningManagerComponent::OnLevelAdded(ULevel* InLevel, UWorld* InWorld)
{
	if (InWorld == GetWorld())
	{
		for (AActor* Actor : InLevel->Actors)
		{
			if (AN1PlayerStart* PlayerStart = Cast<AN1PlayerStart>(Actor))
			{
				ensure(!CachedPlayerStarts.Contains(PlayerStart));
				CachedPlayerStarts.Add(PlayerStart);
			}
		}
	}
}

void UN1PlayerSpawningManagerComponent::HandleOnActorSpawned(AActor* SpawnedActor)
{
	if (AN1PlayerStart* PlayerStart = Cast<AN1PlayerStart>(SpawnedActor))
	{
		CachedPlayerStarts.Add(PlayerStart);
	}
}

#if WITH_EDITOR
APlayerStart* UN1PlayerSpawningManagerComponent::FindPlayFromHereStart(AController* Player)
{
	// Only 'Play From Here' for a player controller, bots etc. should all spawn from normal spawn points.
	if (Player->IsA<APlayerController>())
	{
		if (UWorld* World = GetWorld())
		{
			for (TActorIterator<APlayerStart> It(World); It; ++It)
			{
				if (APlayerStart* PlayerStart = *It)
				{
					if (PlayerStart->IsA<APlayerStartPIE>())
					{
						// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
						return PlayerStart;
					}
				}
			}
		}
	}

	return nullptr;
}
#endif
