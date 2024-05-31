// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerState.h"
#include "Player/N1PlayerController.h"
#include "Game/N1ExperienceManagerComponent.h"
#include "Game/N1GameMode.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "AbilitySystem/N1AbilitySet.h"
#include "AbilitySystem/Attributes/N1CombatSet.h"
#include "AbilitySystem/Attributes/N1HealthSet.h"
#include "Character/N1PawnData.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "N1LogChannels.h"
#include <Character/N1PawnExtensionComponent.h>
#include "Components/GameFrameworkComponentManager.h"
#include "Messages/N1VerbMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1PlayerState)

const FName AN1PlayerState::NAME_N1AbilityReady("N1AbilitiesReady");

AN1PlayerState::AN1PlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UN1AbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<UN1HealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UN1CombatSet>(TEXT("CombatSet"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;

	MyTeamID = FGenericTeamId::NoTeam;
	MySquadID = INDEX_NONE;
}

AN1PlayerController* AN1PlayerState::GetN1PlayerController() const
{
	return Cast<AN1PlayerController>(GetOwner());
}

void AN1PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MySquadID, SharedParams);

	SharedParams.Condition = ELifetimeCondition::COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ReplicatedViewRotation, SharedParams);

	DOREPLIFETIME(ThisClass, StatTags);
}

void AN1PlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AN1PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	{
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UN1ExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UN1ExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnN1ExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AN1PlayerState::Reset()
{
	Super::Reset();
}

void AN1PlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void AN1PlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AN1PlayerState::OnDeactivated()
{
	bool bDestroyDeactivatedPlayerState = false;

	switch (GetPlayerConnectionType())
	{
	case EN1PlayerConnectionType::Player:
	case EN1PlayerConnectionType::InactivePlayer:
		//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
		// (e.g., for long running servers where they might build up if lots of players cycle through)
		bDestroyDeactivatedPlayerState = true;
		break;
	default:
		bDestroyDeactivatedPlayerState = true;
		break;
	}

	SetPlayerConnectionType(EN1PlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void AN1PlayerState::OnReactivated()
{
	if (GetPlayerConnectionType() == EN1PlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(EN1PlayerConnectionType::Player);
	}
}

void AN1PlayerState::SetPlayerConnectionType(EN1PlayerConnectionType NewType)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyPlayerConnectionType, this);
	MyPlayerConnectionType = NewType;
}

void AN1PlayerState::OnExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience)
{
	if (AN1GameMode* GameMode = GetWorld()->GetAuthGameMode<AN1GameMode>())
	{
		const UN1PawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);
		SetPawnData(NewPawnData);
	}
}

void AN1PlayerState::SetPawnData(const UN1PawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogN1, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UN1AbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_N1AbilityReady);

	ForceNetUpdate();
}

UAbilitySystemComponent* AN1PlayerState::GetAbilitySystemComponent() const
{
	return GetN1AbilitySystemComponent();
}

void AN1PlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	}
	else
	{
		UE_LOG(LogN1, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
	}
}

FGenericTeamId AN1PlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnN1TeamIndexChangedDelegate* AN1PlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AN1PlayerState::SetSquadID(int32 NewSquadID)
{
	if (HasAuthority())
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MySquadID, this);

		MySquadID = NewSquadID;
	}
}

void AN1PlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void AN1PlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 AN1PlayerState::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool AN1PlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void AN1PlayerState::ClientBroadcastMessage_Implementation(const FN1VerbMessage Message)
{
	// This check is needed to prevent running the action when in standalone mode
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

FRotator AN1PlayerState::GetReplicatedViewRotation() const
{
	return ReplicatedViewRotation;
}

void AN1PlayerState::SetReplicatedViewRotation(const FRotator& NewRotation)
{
	if (NewRotation != ReplicatedViewRotation)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, ReplicatedViewRotation, this);
		ReplicatedViewRotation = NewRotation;
	}
}

void AN1PlayerState::OnRep_PawnData()
{
}


void AN1PlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AN1PlayerState::OnRep_MySquadID()
{
	// TODO
}
