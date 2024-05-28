// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/N1TeamInfoBase.h"

#include "Teams/N1TeamSubsystem.h"
#include <Net/UnrealNetwork.h>

AN1TeamInfoBase::AN1TeamInfoBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TeamId(INDEX_NONE)
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetPriority = 3.0f;
	SetReplicatingMovement(false);
}

void AN1TeamInfoBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTags);
	DOREPLIFETIME_CONDITION(ThisClass, TeamId, COND_InitialOnly);
}

void AN1TeamInfoBase::BeginPlay()
{
	Super::BeginPlay();

	TryRegisterWithTeamSubsystem();
}

void AN1TeamInfoBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TeamId != INDEX_NONE)
	{
		UN1TeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UN1TeamSubsystem>();
		if (TeamSubsystem)
		{
			// EndPlay can happen at weird times where the subsystem has already been destroyed
			TeamSubsystem->UnregisterTeamInfo(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void AN1TeamInfoBase::RegisterWithTeamSubsystem(UN1TeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this);
}

void AN1TeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamId != INDEX_NONE)
	{
		UN1TeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UN1TeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			RegisterWithTeamSubsystem(TeamSubsystem);
		}
	}
}

void AN1TeamInfoBase::SetTeamId(int32 NewTeamId)
{
	check(HasAuthority());
	check(TeamId == INDEX_NONE);
	check(NewTeamId != INDEX_NONE);

	TeamId = NewTeamId;

	TryRegisterWithTeamSubsystem();
}

void AN1TeamInfoBase::OnRep_TeamId()
{
	TryRegisterWithTeamSubsystem();
}
