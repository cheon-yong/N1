// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1InventoryItemDefinition.h"

#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1InventoryItemDefinition)

//////////////////////////////////////////////////////////////////////
// UN1InventoryItemDefinition

UN1InventoryItemDefinition::UN1InventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UN1InventoryItemFragment* UN1InventoryItemDefinition::FindFragmentByClass(TSubclassOf<UN1InventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
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

//////////////////////////////////////////////////////////////////////
// UN1InventoryItemDefinition

const UN1InventoryItemFragment* UN1InventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UN1InventoryItemDefinition> ItemDef, TSubclassOf<UN1InventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UN1InventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

