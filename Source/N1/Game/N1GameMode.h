// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "N1GameMode.generated.h"

UCLASS(minimalapi)
class AN1GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AN1GameMode();
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



