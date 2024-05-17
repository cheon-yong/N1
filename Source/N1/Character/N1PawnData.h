// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "N1PawnData.generated.h"

class UN1InputConfig;
class UN1CameraMode;

/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "N1 Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class N1_API UN1PawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UN1PawnData(const FObjectInitializer& ObjectInitializer);

	// Class to instantiate for this pawn (should usually derive from ALyraPawn or ALyraCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|Pawn")
	TSubclassOf<APawn> PawnClass;

	//// Ability sets to grant to this pawn's ability system.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|Abilities")
	//TArray<TObjectPtr<ULyraAbilitySet>> AbilitySets;

	//// What mapping of ability tags to use for actions taking by this pawn
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|Abilities")
	//TObjectPtr<ULyraAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|Input")
	TObjectPtr<UN1InputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|Camera")
	TSubclassOf<UN1CameraMode> DefaultCameraMode;
};
