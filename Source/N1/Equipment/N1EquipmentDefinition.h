// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "N1EquipmentDefinition.generated.h"

class UN1EquipmentInstance;

USTRUCT()
struct FN1EquipmentActorToSpawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class N1_API UN1EquipmentDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UN1EquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UN1EquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FN1EquipmentActorToSpawn> ActorsToSpawn;
	
};
