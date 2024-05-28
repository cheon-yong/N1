// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameStateComponent.h"

#include "N1TeamCreationComponent.generated.h"

class UN1ExperienceDefinition;
class AN1TeamPublicInfo;
class AN1TeamPrivateInfo;
class AN1PlayerState;
class AGameModeBase;
class APlayerController;
class UN1TeamDisplayAsset;

/**
 * 
 */
UCLASS(Blueprintable)
class N1_API UN1TeamCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	UN1TeamCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~End of UObject interface

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

private:
	void OnExperienceLoaded(const UN1ExperienceDefinition* Experience);

protected:
	// List of teams to create (id to display asset mapping, the display asset can be left unset if desired)
	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TMap<uint8, TObjectPtr<UN1TeamDisplayAsset>> TeamsToCreate;

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TSubclassOf<AN1TeamPublicInfo> PublicTeamInfoClass;

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TSubclassOf<AN1TeamPrivateInfo> PrivateTeamInfoClass;

#if WITH_SERVER_CODE
protected:
	virtual void ServerCreateTeams();
	virtual void ServerAssignPlayersToTeams();

	/** Sets the team ID for the given player state. Spectator-only player states will be stripped of any team association. */
	virtual void ServerChooseTeamForPlayer(AN1PlayerState* PS);

private:
	void OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer);
	void ServerCreateTeam(int32 TeamId, UN1TeamDisplayAsset* DisplayAsset);

	/** returns the Team ID with the fewest active players, or INDEX_NONE if there are no valid teams */
	int32 GetLeastPopulatedTeamID() const;
#endif
};
