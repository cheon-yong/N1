// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/N1QuickBarComponent.h"
#include "Equipment/N1EquipmentManagerComponent.h"
#include "Equipment/N1EquipmentInstance.h"
#include "Inventory/N1InventoryFragment_EquippableItem.h"
#include "Inventory/N1InventoryItemInstance.h"


UN1QuickBarComponent::UN1QuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1QuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

void UN1QuickBarComponent::AddItemToSlot(int32 SlotIndex, UN1InventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UN1QuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}

UN1EquipmentManagerComponent* UN1QuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UN1EquipmentManagerComponent>();
		}
	}
	return nullptr;

}

void UN1QuickBarComponent::UnequipItemInSlot()
{
	if(UN1EquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UN1QuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);
	if (UN1InventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UN1InventoryItemFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UN1InventoryItemFragment_EquippableItem>())
		{
			TSubclassOf<UN1EquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				if (UN1EquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}
