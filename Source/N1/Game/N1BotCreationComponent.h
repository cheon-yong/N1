// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameStateComponent.h"

#include "N1BotCreationComponent.generated.h"


class UN1ExperienceDefinition;
class UN1PawnData;
class AAIController;

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class N1_API UN1BotCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	UN1BotCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

private:
	void OnExperienceLoaded(const UN1ExperienceDefinition* Experience);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	int32 NumBotsToCreate = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	TSubclassOf<AAIController> BotControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
	TArray<FString> RandomBotNames;

	TArray<FString> RemainingBotNames;

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AAIController>> SpawnedBotList;

	/** Always creates a single bot */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Gameplay)
	virtual void SpawnOneBot();

	/** Deletes the last created bot if possible */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Gameplay)
	virtual void RemoveOneBot();

	/** Spawns bots up to NumBotsToCreate */
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = Gameplay)
	void ServerCreateBots();

#if WITH_SERVER_CODE
public:
	void Cheat_AddBot() { SpawnOneBot(); }
	void Cheat_RemoveBot() { RemoveOneBot(); }

	FString CreateBotName(int32 PlayerIndex);
#endif
	
};
