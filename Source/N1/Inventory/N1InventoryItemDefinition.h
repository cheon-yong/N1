// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "N1InventoryItemDefinition.generated.h"


UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class UN1InventoryItemFragment : public UObject
{
	GENERATED_BODY()
	public:
};


/**
 * 
 */
UCLASS(Blueprintable)
class N1_API UN1InventoryItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UN1InventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	const UN1InventoryItemFragment* FindFragmentByClass(TSubclassOf<UN1InventoryItemFragment> FragmentClass) const;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<UN1InventoryItemFragment>> Fragments;

};
