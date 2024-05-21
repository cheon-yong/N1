// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/N1InventoryManagerComponent.h"
#include "Inventory/N1InventoryItemDefinition.h"
#include "Inventory/N1InventoryItemInstance.h"

UN1InventoryManagerComponent::UN1InventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}

UN1InventoryItemInstance* UN1InventoryManagerComponent::AddItemDefinition(TSubclassOf<UN1InventoryItemDefinition> ItemDef)
{
	UN1InventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}

UN1InventoryItemInstance* FN1InventoryList::AddEntry(TSubclassOf<UN1InventoryItemDefinition> ItemDef)
{
	UN1InventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);
	
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	
	FN1InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UN1InventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;
	Result = NewEntry.Instance;
	return Result;
}
