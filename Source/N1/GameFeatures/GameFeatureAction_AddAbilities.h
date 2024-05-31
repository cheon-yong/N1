// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFeatures/GameFeatureAction_WorldAction.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/N1AbilitySet.h"

#include "GameFeatureAction_AddAbilities.generated.h"

struct FWorldContext;
class UInputAction;
class UAttributeSet;
class UDataTable;
struct FComponentRequestHandle;
class UN1AbilitySet;

USTRUCT(BlueprintType)
struct FN1AbilityGrant
{
	GENERATED_BODY()

	// Type of ability to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "Client,Server"))
	TSoftClassPtr<UGameplayAbility> AbilityType;

	// Input action to bind the ability to, if any (can be left unset)
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	TSoftObjectPtr<UInputAction> InputAction;
};

USTRUCT(BlueprintType)
struct FN1AttributeSetGrant
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "Client,Server"))
	TSoftClassPtr<UAttributeSet> AttributeSetType;

	// Data table referent to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "Client,Server"))
	TSoftObjectPtr<UDataTable> InitializationData;
};

USTRUCT()
struct FGameFeatureAbilitiesEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSoftClassPtr<AActor> ActorClass;

	// List of abilities to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FN1AbilityGrant> GrantedAbilities;

	// List of attribute sets to grant to actors of the specified class 
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<FN1AttributeSetGrant> GrantedAttributes;

	// List of ability sets to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Attributes", meta = (AssetBundles = "Client,Server"))
	TArray<TSoftObjectPtr<const UN1AbilitySet>> GrantedAbilitySets;
};

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Abilities"))
class UGameFeatureAction_AddAbilities : public UGameFeatureAction_WorldAction
{
	GENERATED_BODY()
	
public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ End UObject interface




	/**  */
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAbilitiesEntry> AbilitiesList;

private:
	struct FActorExtensions
	{
		TArray<FGameplayAbilitySpecHandle> Abilities;
		TArray<UAttributeSet*> Attributes;
		TArray<FN1AbilitySet_GrantedHandles> AbilitySetHandles;
	};

	struct FPerContextData
	{
		TMap<AActor*, FActorExtensions> ActiveExtensions;
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

	void Reset(FPerContextData& ActiveData);
	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex, FGameFeatureStateChangeContext ChangeContext);
	void AddActorAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData);
	void RemoveActorAbilities(AActor* Actor, FPerContextData& ActiveData);

	template<class ComponentType>
	ComponentType* FindOrAddComponentForActor(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData)
	{
		//@TODO: Just find, no add?
		return Cast<ComponentType>(FindOrAddComponentForActor(ComponentType::StaticClass(), Actor, AbilitiesEntry, ActiveData));
	}
	UActorComponent* FindOrAddComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData);
};
