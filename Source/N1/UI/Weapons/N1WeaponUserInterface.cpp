// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1WeaponUserInterface.h"

#include "Equipment/N1EquipmentManagerComponent.h"
#include "GameFramework/Pawn.h"
#include "Weapons/N1WeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1WeaponUserInterface)

struct FGeometry;

UN1WeaponUserInterface::UN1WeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1WeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void UN1WeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
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
					RebuildWidgetFromWeapon();
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}

void UN1WeaponUserInterface::RebuildWidgetFromWeapon()
{

}