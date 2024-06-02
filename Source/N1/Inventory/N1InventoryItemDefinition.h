// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "N1InventoryItemDefinition.generated.h"

template <typename T> class TSubclassOf;

class UN1InventoryItemInstance;
struct FFrame;

//////////////////////////////////////////////////////////////////////

// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class N1_API UN1InventoryItemFragment : public UObject
{
	GENERATED_BODY()

	public:
	virtual void OnInstanceCreated(UN1InventoryItemInstance* Instance) const {}
};

//////////////////////////////////////////////////////////////////////

/**
 * UN1InventoryItemDefinition
 */
UCLASS(Blueprintable, Const, Abstract)
class UN1InventoryItemDefinition : public UObject
{
	GENERATED_BODY()

	public:
	UN1InventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, Instanced)
	TArray<TObjectPtr<UN1InventoryItemFragment>> Fragments;

public:
	const UN1InventoryItemFragment* FindFragmentByClass(TSubclassOf<UN1InventoryItemFragment> FragmentClass) const;
};

//@TODO: Make into a subsystem instead?
UCLASS()
class UN1InventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = FragmentClass))
	static const UN1InventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UN1InventoryItemDefinition> ItemDef, TSubclassOf<UN1InventoryItemFragment> FragmentClass);
};
