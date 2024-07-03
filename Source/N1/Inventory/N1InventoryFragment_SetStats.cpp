// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1InventoryFragment_SetStats.h"

#include "Inventory/N1InventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1InventoryFragment_SetStats)

void UN1InventoryFragment_SetStats::OnInstanceCreated(UN1InventoryItemInstance* Instance) const
{
	for (const auto& KVP : InitialItemStats)
	{
		Instance->AddStatTagStack(KVP.Key, KVP.Value);
	}
}

int32 UN1InventoryFragment_SetStats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
