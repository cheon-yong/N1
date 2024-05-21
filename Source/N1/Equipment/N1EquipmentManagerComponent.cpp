// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/N1EquipmentManagerComponent.h"
#include "Equipment/N1EquipmentInstance.h"

UN1EquipmentManagerComponent::UN1EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
}

UN1EquipmentInstance* UN1EquipmentManagerComponent::EquipItem(TSubclassOf<UN1EquipmentDefinition> EquipmentDefinition)
{
	UN1EquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

void UN1EquipmentManagerComponent::UnequipItem(UN1EquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

UN1EquipmentInstance* FN1EquipmentList::AddEntry(TSubclassOf<UN1EquipmentDefinition> EquipmentDefinition)
{
	UN1EquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	const UN1EquipmentDefinition* EquipmentCDO = GetDefault<UN1EquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<UN1EquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UN1EquipmentInstance::StaticClass();
	}
	FN1AppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UN1EquipmentInstance>(OwnerComponent->GetOwner(),
		InstanceType);
	Result = NewEntry.Instance;

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
	return Result;
}

void FN1EquipmentList::RemoveEntry(UN1EquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FN1AppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}
