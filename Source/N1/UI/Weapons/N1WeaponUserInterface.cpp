// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/N1WeaponUserInterface.h"
#include "Equipment/N1EquipmentManagerComponent.h"
#include "Weapons/N1WeaponInstance.h"

UN1WeaponUserInterface::UN1WeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1WeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UN1EquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UN1EquipmentManagerComponent>())
		{
			if (UN1WeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UN1WeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					UN1WeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}
