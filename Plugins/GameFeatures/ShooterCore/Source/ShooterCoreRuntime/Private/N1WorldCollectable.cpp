// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1WorldCollectable.h"

#include "Async/TaskGraphInterfaces.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1WorldCollectable)

struct FInteractionQuery;

AN1WorldCollectable::AN1WorldCollectable()
{
}

void AN1WorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup AN1WorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
