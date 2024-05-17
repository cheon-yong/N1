// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "N1GameMode.generated.h"

// forward declaration
class UN1ExperienceDefinition;

UCLASS()
class AN1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AN1GameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// About Lyra Experience
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override final;
	void HandleMatchAssignmentIfNotExpectingOne();
	virtual void InitGameState() override final;
	void OnExperienceLoaded(const UN1ExperienceDefinition* CurrentExperience);
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform&SpawnTransform) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	bool IsExperienceLoaded() const;
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	UFUNCTION(BlueprintCallable, Category = "N1|Pawn")
	const UN1PawnData* GetPawnDataForController(const AController* InController) const;

	virtual FTransform GetRandomStartTransform() const;
	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

	//virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	//virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	//virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void DefaultGameTimer();
	void FinishMatch();

	FTimerHandle GameTimerHandle;

protected:
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;
};



