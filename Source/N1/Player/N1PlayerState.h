// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Teams/N1TeamAgentInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.h"
#include "System/GameplayTagStack.h"

#include "N1PlayerState.generated.h"

// forward declaration
class UN1ExperienceDefinition;
class UN1PawnData;
class UN1AbilitySystemComponent;
class UN1HealthSet;
class AN1PlayerController;
class AController;
class APlayerState;
class FName;
class UAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FN1VerbMessage;

UENUM()
enum class EN1PlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};


/**
 * 
 */
UCLASS()
class N1_API AN1PlayerState : public AModularPlayerState, public IAbilitySystemInterface, public IN1TeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AN1PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "N1|PlayerState")
	AN1PlayerController* GetN1PlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "N1|PlayerState")
	UN1AbilitySystemComponent* GetN1AbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override final;

	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface

	void SetPlayerConnectionType(EN1PlayerConnectionType NewType);
	EN1PlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }

	void OnExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience);

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UN1PawnData* InPawnData);

	//~IN1TeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnN1TeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IN1TeamAgentInterface interface

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

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category = Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category = Teams)
	bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "N1|PlayerState")
	void ClientBroadcastMessage(const FN1VerbMessage Message);

	// Gets the replicated view rotation of this player, used for spectating
	FRotator GetReplicatedViewRotation() const;

	// Sets the replicated view rotation, only valid on the server
	void SetReplicatedViewRotation(const FRotator& NewRotation);

protected:
	UFUNCTION()
	void OnRep_PawnData();

private:
	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UFUNCTION()
	void OnRep_MySquadID();

public:
	static const FName NAME_N1AbilityReady;

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UN1PawnData> PawnData;

private:
	UPROPERTY(VisibleAnywhere, Category = "N1|PlayerState")
	TObjectPtr<UN1AbilitySystemComponent> AbilitySystemComponent;

	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UN1HealthSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UN1CombatSet> CombatSet;

	UPROPERTY(Replicated)
	EN1PlayerConnectionType MyPlayerConnectionType;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY(Replicated)
	FRotator ReplicatedViewRotation;

	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY(ReplicatedUsing = OnRep_MySquadID)
	int32 MySquadID;

	UPROPERTY()
	FOnN1TeamIndexChangedDelegate OnTeamChangedDelegate;
};
