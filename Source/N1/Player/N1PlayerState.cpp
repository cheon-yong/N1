// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/N1PlayerState.h"
#include "Game/N1ExperienceManagerComponent.h"
#include "Game/N1GameMode.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "AbilitySystem/N1AbilitySet.h"
#include "Character/N1PawnData.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"


AN1PlayerState::AN1PlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UN1AbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}

void AN1PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	//DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	//DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MySquadID, SharedParams);

	SharedParams.Condition = ELifetimeCondition::COND_SkipOwner;
	//DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ReplicatedViewRotation, SharedParams);

	//DOREPLIFETIME(ThisClass, StatTags);
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
	check(!PawnData);

	PawnData = InPawnData;
	for (UN1AbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

UAbilitySystemComponent* AN1PlayerState::GetAbilitySystemComponent() const
{
	return GetN1AbilitySystemComponent();
}

void AN1PlayerState::SetSquadID(int32 NewSquadID)
{
	if (HasAuthority())
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MySquadID, this);

		MySquadID = NewSquadID;
	}
}

void AN1PlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AN1PlayerState::OnRep_MySquadID()
{
	// TODO
}
