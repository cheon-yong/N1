// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1GameplayAbility_FromEquipment.h"
#include "N1EquipmentInstance.h"
#include "Inventory/N1InventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameplayAbility_FromEquipment)

UN1GameplayAbility_FromEquipment::UN1GameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UN1EquipmentInstance* UN1GameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UN1EquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UN1InventoryItemInstance* UN1GameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UN1EquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UN1InventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UN1GameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Context.AddError(NSLOCTEXT("N1", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
