// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/N1TeamPublicInfo.h"

#include "Teams/N1TeamInfoBase.h"
#include "Net/UnrealNetwork.h"

class FLifetimeProperty;

AN1TeamPublicInfo::AN1TeamPublicInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AN1TeamPublicInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void AN1TeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}

void AN1TeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UN1TeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}
