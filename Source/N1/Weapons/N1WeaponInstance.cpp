// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/N1WeaponInstance.h"
#include "Cosmetics/N1CharacterPartTypes.h"
#include "Character/N1HealthComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputDeviceProperties.h"



UN1WeaponInstance::UN1WeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Listen for death of the owning pawn so that any device properties can be removed if we
	// die and can't unequip
	if (APawn* Pawn = GetPawn())
	{
		// We only need to do this for player controlled pawns, since AI and others won't have input devices on the client
		if (Pawn->IsPlayerControlled())
		{
			if (UN1HealthComponent* HealthComponent = UN1HealthComponent::FindHealthComponent(GetPawn()))
			{
				HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
			}
		}
	}
}

void UN1WeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	UWorld* World = GetWorld();
	check(World);
	TimeLastEquipped = World->GetTimeSeconds();

	ApplyDeviceProperties();
}

void UN1WeaponInstance::OnUnequipped()
{
	Super::OnUnequipped();

	RemoveDeviceProperties();
}

void UN1WeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	TimeLastFired = World->GetTimeSeconds();
}

float UN1WeaponInstance::GetTimeSinceLastInteractedWith() const
{
	UWorld* World = GetWorld();
	check(World);
	const double WorldTime = World->GetTimeSeconds();

	double Result = WorldTime - TimeLastEquipped;

	if (TimeLastFired > 0.0)
	{
		const double TimeSinceFired = WorldTime - TimeLastFired;
		Result = FMath::Min(Result, TimeSinceFired);
	}

	return Result;
}

TSubclassOf<UAnimInstance> UN1WeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FN1AnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}

const FPlatformUserId UN1WeaponInstance::GetOwningUserId() const
{
	if (const APawn* Pawn = GetPawn())
	{
		return Pawn->GetPlatformUserId();
	}
	return PLATFORMUSERID_NONE;
}

void UN1WeaponInstance::OnDeathStarted(AActor* OwningActor)
{
	RemoveDeviceProperties();
}

void UN1WeaponInstance::ApplyDeviceProperties()
{
	const FPlatformUserId UserId = GetOwningUserId();

	if (UserId.IsValid())
	{
		if (UInputDeviceSubsystem* InputDeviceSubsystem = UInputDeviceSubsystem::Get())
		{
			for (TObjectPtr<UInputDeviceProperty>& DeviceProp : ApplicableDeviceProperties)
			{
				FActivateDevicePropertyParams Params = {};
				Params.UserId = UserId;

				// By default, the device property will be played on the Platform User's Primary Input Device.
				// If you want to override this and set a specific device, then you can set the DeviceId parameter.
				//Params.DeviceId = <some specific device id>;

				// Don't remove this property it was evaluated. We want the properties to be applied as long as we are holding the 
				// weapon, and will remove them manually in OnUnequipped
				Params.bLooping = true;

				DevicePropertyHandles.Emplace(InputDeviceSubsystem->ActivateDeviceProperty(DeviceProp, Params));
			}
		}
	}
}

void UN1WeaponInstance::RemoveDeviceProperties()
{
	const FPlatformUserId UserId = GetOwningUserId();

	if (UserId.IsValid() && !DevicePropertyHandles.IsEmpty())
	{
		// Remove any device properties that have been applied
		if (UInputDeviceSubsystem* InputDeviceSubsystem = UInputDeviceSubsystem::Get())
		{
			InputDeviceSubsystem->RemoveDevicePropertyHandles(DevicePropertyHandles);
			DevicePropertyHandles.Empty();
		}
	}
}
