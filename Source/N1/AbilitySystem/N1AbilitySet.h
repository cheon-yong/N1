// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"


#include "N1AbilitySet.generated.h"

class UN1GameplayAbility;
class UN1AbilitySystemComponent;

USTRUCT(BlueprintType)
struct FN1AbilitySet_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UN1GameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FN1AbilitySet_GrantedHandles
{
	GENERATED_BODY()
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	
	void TakeFromAbilitySystem(UN1AbilitySystemComponent* ASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

/**
 * 
 */
UCLASS(BlueprintType)
class N1_API UN1AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UN1AbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UN1AbilitySystemComponent* ASC, FN1AbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FN1AbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
