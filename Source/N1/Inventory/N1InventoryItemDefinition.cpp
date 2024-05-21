// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/N1InventoryItemDefinition.h"

UN1InventoryItemDefinition::UN1InventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UN1InventoryItemFragment* UN1InventoryItemDefinition::FindFragmentByClass(TSubclassOf<UN1InventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (UN1InventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
