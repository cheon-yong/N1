// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Teams/N1TeamAgentInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.h"

#include "N1PlayerState.generated.h"

// forward declaration
class UN1ExperienceDefinition;
class UN1PawnData;
class UN1AbilitySystemComponent;
class UN1HealthSet;

/**
 * 
 */
UCLASS()
class N1_API AN1PlayerState : public AModularPlayerState, public IAbilitySystemInterface, public IN1TeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AN1PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override final;

	void OnExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience);

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UN1PawnData* InPawnData);

	UN1AbilitySystemComponent* GetN1AbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns the Squad ID of the squad the player belongs to. */
	UFUNCTION(BlueprintCallable)
	int32 GetSquadId() const
	{
		return MySquadID;
	}

	void SetSquadID(int32 NewSquadID);

	/** Returns the Team ID of the team the player belongs to. */
	UFUNCTION(BlueprintCallable)
	int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}

private:
	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UFUNCTION()
	void OnRep_MySquadID();

public:
	UPROPERTY()
	TObjectPtr<const UN1PawnData> PawnData;

	

private:
	UPROPERTY(VisibleAnywhere, Category = "N1|PlayerState")
	TObjectPtr<UN1AbilitySystemComponent> AbilitySystemComponent;

	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UN1HealthSet> HealthSet;
	// Combat attribute set used by this actor.
	/*UPROPERTY()
	TObjectPtr<const class UN1CombatSet> CombatSet;*/

	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY(ReplicatedUsing = OnRep_MySquadID)
	int32 MySquadID;

	UPROPERTY()
	FOnN1TeamIndexChangedDelegate OnTeamChangedDelegate;
};
