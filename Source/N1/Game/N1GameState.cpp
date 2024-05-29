// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/N1GameState.h"

#include "Game/N1ExperienceManagerComponent.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "Player/N1PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "N1LogChannels.h"
#include <GameFramework/GameplayMessageSubsystem.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameState)

class APlayerState;
class FLifetimeProperty;

extern ENGINE_API float GAverageFPS;

AN1GameState::AN1GameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UN1ExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UN1AbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ServerFPS = 0.0f;
}

void AN1GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerFPS);
	DOREPLIFETIME_CONDITION(ThisClass, RecorderPlayerState, COND_ReplayOnly);
}

void AN1GameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AN1GameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

void AN1GameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AN1GameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		ServerFPS = GAverageFPS;
	}
}

void AN1GameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void AN1GameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

void AN1GameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap)
{
	// Remove inactive and bots
	for (int32 i = PlayerArray.Num() - 1; i >= 0; i--)
	{
		APlayerState* PlayerState = PlayerArray[i];
		if (PlayerState && (PlayerState->IsABot() || PlayerState->IsInactive()))
		{
			RemovePlayerState(PlayerState);
		}
	}
}

UAbilitySystemComponent* AN1GameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AN1GameState::MulticastMessageToClients_Implementation(const FN1VerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void AN1GameState::MulticastReliableMessageToClients_Implementation(const FN1VerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}

float AN1GameState::GetServerFPS() const
{
	return ServerFPS;
}

void AN1GameState::SetRecorderPlayerState(APlayerState* NewPlayerState)
{
	if (RecorderPlayerState == nullptr)
	{
		// Set it and call the rep callback so it can do any record-time setup
		RecorderPlayerState = NewPlayerState;
		OnRep_RecorderPlayerState();
	}
	else
	{
		UE_LOG(LogN1, Warning, TEXT("SetRecorderPlayerState was called on %s but should only be called once per game on the primary user"), *GetName());
	}
}

APlayerState* AN1GameState::GetRecorderPlayerState() const
{
	return RecorderPlayerState;
}

void AN1GameState::OnRep_RecorderPlayerState()
{
	OnRecorderPlayerStateChangedEvent.Broadcast(RecorderPlayerState);
}
