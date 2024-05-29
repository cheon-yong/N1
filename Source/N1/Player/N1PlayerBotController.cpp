// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerBotController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Perception/AIPerceptionComponent.h"
#include "Game/N1GameMode.h"
#include "Teams/N1TeamAgentInterface.h"
#include "N1LogChannels.h"
#include "GameFramework/PlayerState.h"

AN1PlayerBotController::AN1PlayerBotController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsPlayerState = true;
	bStopAILogicOnUnposses = false;
}

void AN1PlayerBotController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogN1, Error, TEXT("You can't set the team ID on a player bot controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
}

FGenericTeamId AN1PlayerBotController::GetGenericTeamId() const
{
	if (IN1TeamAgentInterface* PSWithTeamInterface = Cast<IN1TeamAgentInterface>(PlayerState))
	{
		return PSWithTeamInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

FOnN1TeamIndexChangedDelegate* AN1PlayerBotController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

ETeamAttitude::Type AN1PlayerBotController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IN1TeamAgentInterface* TeamAgent = Cast<IN1TeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();

			//Checking Other pawn ID to define Attitude
			if (OtherTeamID.GetId() != GetGenericTeamId().GetId())
			{
				return ETeamAttitude::Hostile;
			}
			else
			{
				return ETeamAttitude::Friendly;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

void AN1PlayerBotController::ServerRestartController()
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	ensure((GetPawn() == nullptr) && IsInState(NAME_Inactive));

	if (IsInState(NAME_Inactive) || (IsInState(NAME_Spectating)))
	{
		AN1GameMode* const GameMode = GetWorld()->GetAuthGameMode<AN1GameMode>();

		if ((GameMode == nullptr) || !GameMode->ControllerCanRestart(this))
		{
			return;
		}

		// If we're still attached to a Pawn, leave it
		if (GetPawn() != nullptr)
		{
			UnPossess();
		}

		// Re-enable input, similar to code in ClientRestart
		ResetIgnoreInputFlags();

		GameMode->RestartPlayer(this);
	}
}

void AN1PlayerBotController::UpdateTeamAttitude(UAIPerceptionComponent* AIPerception)
{
	if (AIPerception)
	{
		AIPerception->RequestStimuliListenerUpdate();
	}
}

void AN1PlayerBotController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}

void AN1PlayerBotController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}

void AN1PlayerBotController::OnPlayerStateChanged()
{
}

void AN1PlayerBotController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();

	// Unbind from the old player state, if any
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (IN1TeamAgentInterface* PlayerStateTeamInterface = Cast<IN1TeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (IN1TeamAgentInterface* PlayerStateTeamInterface = Cast<IN1TeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	// Broadcast the team change (if it really has)
	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	LastSeenPlayerState = PlayerState;
}

void AN1PlayerBotController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AN1PlayerBotController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AN1PlayerBotController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}
