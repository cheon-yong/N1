// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "N1InventoryItemInstance.generated.h"

/**
 * 
 */
class UN1InventoryItemDefinition;
class UN1InventoryItemFragment;

UCLASS(BlueprintType)
class N1_API UN1InventoryItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UN1InventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UN1InventoryItemFragment* FindFragmentByClass(TSubclassOf<UN1InventoryItemFragment> FragmentClass) const;
	
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}


	UPROPERTY()
	TSubclassOf<UN1InventoryItemDefinition> ItemDef;
};
