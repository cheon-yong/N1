// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/N1GameplayAbility_FromEquipment.h"
#include "N1GameplayAbility_RangedWeapon.generated.h"

class UN1RangedWeaponInstance;
class UN1EquipmentInstance;

UENUM(BlueprintType)
enum class EN1AbilityTargetingSource : uint8
{
	CameraTowardsFocus,
};


/**
 * 
 */
UCLASS()
class N1_API UN1GameplayAbility_RangedWeapon : public UN1GameplayAbility_FromEquipment
{
	GENERATED_BODY()
	
	struct FRangedWeaponFiringInput
	{
		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{}

		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		UN1RangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;
	};
	
	
public:
	UN1GameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	void PerformLocalTargeting(TArray<FHitResult>& OutHits);

	UN1RangedWeaponInstance* GetWeaponInstance();

	UN1EquipmentInstance* GetAssociatedEquipment() const;

	FVector GetWeaponTargetingSourceLocation() const;

	FTransform GetTargetingTransform(APawn* SourcePawn, EN1AbilityTargetingSource Source);

	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);

	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;

	static int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;

	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;

	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
};
