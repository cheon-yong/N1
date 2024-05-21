// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/N1EquipmentDefinition.h"
#include "Equipment/N1EquipmentInstance.h"

UN1EquipmentDefinition::UN1EquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UN1EquipmentInstance::StaticClass();
}
