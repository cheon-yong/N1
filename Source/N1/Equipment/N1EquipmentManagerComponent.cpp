// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/N1EquipmentManagerComponent.h"
#include "Equipment/N1EquipmentInstance.h"
#include <AbilitySystemGlobals.h>
#include "AbilitySystem/N1AbilitySystemComponent.h"

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

TArray<UN1EquipmentInstance*> UN1EquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UN1EquipmentInstance> InstanceType) const
{
	TArray<UN1EquipmentInstance*> Results;
	for (const FN1AppliedEquipmentEntry& Entry : EquipmentList.Entries)
		if (UN1EquipmentInstance* Instance = Entry.Instance)
			if (Instance->IsA(InstanceType))
				Results.Add(Instance);

	return Results;
}

UN1EquipmentInstance* UN1EquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<ULyraCloneEquipmentInstance> InstanceType)
{
	for (FN1AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UN1EquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}
	return nullptr;
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

	// About Abiilty
	UN1AbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	{
		for (TObjectPtr<UN1AbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

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
			UN1AbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UN1AbilitySystemComponent* FN1EquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UN1AbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}
