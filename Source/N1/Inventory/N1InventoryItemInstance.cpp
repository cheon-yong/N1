// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/N1InventoryItemInstance.h"
#include "Inventory/N1InventoryItemDefinition.h"

UN1InventoryItemInstance::UN1InventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UN1InventoryItemFragment* UN1InventoryItemInstance::FindFragmentByClass(TSubclassOf<UN1InventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UN1InventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
