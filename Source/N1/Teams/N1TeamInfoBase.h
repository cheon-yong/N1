// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Info.h"
#include "System/GameplayTagStack.h"

#include "N1TeamInfoBase.generated.h"

namespace EEndPlayReason { enum Type : int; }

class UN1TeamCreationComponent;
class UN1TeamSubsystem;
class UObject;
struct FFrame;

/**
 * 
 */
UCLASS(Abstract)
class N1_API AN1TeamInfoBase : public AInfo
{
	GENERATED_BODY()
	
public:
	AN1TeamInfoBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 GetTeamId() const { return TeamId; }

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

protected:
	virtual void RegisterWithTeamSubsystem(UN1TeamSubsystem* Subsystem);
	void TryRegisterWithTeamSubsystem();

private:
	void SetTeamId(int32 NewTeamId);

	UFUNCTION()
	void OnRep_TeamId();

public:
	friend UN1TeamCreationComponent;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TeamTags;

private:
	UPROPERTY(ReplicatedUsing = OnRep_TeamId)
	int32 TeamId;
};
