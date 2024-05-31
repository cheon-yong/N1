// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/N1GameplayAbility_RangedWeapon.h"
#include "Weapons/N1RangedWeaponInstance.h"
#include "Equipment/N1EquipmentInstance.h"
#include <Physics/N1CollisionChannels.h>
#include "AbilitySystemComponent.h"
#include "AbilitySystem/N1GameplayAbilityTargetData_SingleTarget.h"
#include "NativeGameplayTags.h"
#include "N1LogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1GameplayAbility_RangedWeapon)

namespace N1ConsoleVariables
{
	static float DrawBulletTracesDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletTraceDuraton(
		TEXT("N1.Weapon.DrawBulletTraceDuration"),
		DrawBulletTracesDuration,
		TEXT("Should we do debug drawing for bullet traces (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletHitDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHits(
		TEXT("N1.Weapon.DrawBulletHitDuration"),
		DrawBulletHitDuration,
		TEXT("Should we do debug drawing for bullet impacts (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletHitRadius = 3.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHitRadius(
		TEXT("N1.Weapon.DrawBulletHitRadius"),
		DrawBulletHitRadius,
		TEXT("When bullet hit debug drawing is enabled (see DrawBulletHitDuration), how big should the hit radius be? (in uu)"),
		ECVF_Default);
}

// Weapon fire will be blocked/canceled if the player has this tag
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_WeaponFireBlocked, "Ability.Weapon.NoFiring");

//////////////////////////////////////////////////////////////////////

FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		// consider the cone a concatenation of two rotations. one "away" from the center line, and another "around" the circle
		// apply the exponent to the away-from-center rotation. a larger exponent will cluster points more tightly around the center
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Dir.Rotation();
		FQuat DirQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f, 0.0, AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;
		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}


UN1GameplayAbility_RangedWeapon::UN1GameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SourceBlockedTags.AddTag(TAG_WeaponFireBlocked);
}

void UN1GameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
	check(CurrentActorInfo);
	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();

	check(AvatarActor);
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();

	check(MyAbilityComponent);
	TArray<FHitResult> FoundHits;

	PerformLocalTargeting(FoundHits);
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = 0;
	if (FoundHits.Num() > 0)
	{
		const int32 CartridgeID = FMath::Rand();
		for (const FHitResult& FoundHit : FoundHits)
		{
			FN1GameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FN1GameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
			TargetData.Add(NewTargetData);
		}
	}

	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}

void UN1GameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	UN1RangedWeaponInstance* WeaponData = GetWeaponInstance();
	if (AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData)
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = WeaponData;
		InputData.bCanPlayBulletFX = (AvatarPawn->GetNetMode() != NM_DedicatedServer);

		//@TODO: Should do more complicated logic here when the player is close to a wall, etc...
		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn, EN1AbilityTargetingSource::CameraTowardsFocus);
		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();

		InputData.EndAim = InputData.StartTrace + InputData.AimDir * WeaponData->GetMaxDamageRange();

#if ENABLE_DRAW_DEBUG
		if (N1ConsoleVariables::DrawBulletTracesDuration > 0.0f)
		{
			static float DebugThickness = 2.0f;
			DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.0f), FColor::Yellow, false, N1ConsoleVariables::DrawBulletTracesDuration, 0, DebugThickness);
		}
#endif

		TraceBulletsInCartridge(InputData, /*out*/ OutHits);
	}

}

UN1RangedWeaponInstance* UN1GameplayAbility_RangedWeapon::GetWeaponInstance()
{
	return Cast<UN1RangedWeaponInstance>(GetAssociatedEquipment());
}

bool UN1GameplayAbility_RangedWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return false;
}

void UN1GameplayAbility_RangedWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
}

void UN1GameplayAbility_RangedWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

UN1EquipmentInstance* UN1GameplayAbility_RangedWeapon::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UN1EquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;

}

FVector UN1GameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);
	const FVector SourceLoc = AvatarPawn->GetActorLocation();
	return SourceLoc;
}

FTransform UN1GameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, EN1AbilityTargetingSource Source)
{
	check(SourcePawn);
	check(Source == EN1AbilityTargetingSource::CameraTowardsFocus);
	AController* Controller = SourcePawn->Controller;
	if (Controller == nullptr)
	{
		return FTransform();
	}

	double FocalDistance = 1024.0f;
	FVector FocalLoc;

	FVector CamLoc;
	FRotator CamRot;

	APlayerController* PC = Cast<APlayerController>(Controller);
	check(PC);

	PC->GetPlayerViewPoint(CamLoc, CamRot);
	FVector AimDir = CamRot.Vector().GetSafeNormal();
	FocalLoc = CamLoc + (AimDir * FocalDistance);
	const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
	FVector FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);

#if 0
	{
		DrawDebugPoint(GetWorld(), WeaponLoc, 10.0f, FColor::Red, false, 60.0f);
		DrawDebugPoint(GetWorld(), CamLoc, 10.0f, FColor::Yellow, false, 60.0f);
		DrawDebugPoint(GetWorld(), FinalCamLoc, 10.0f, FColor::Magenta, false, 60.0f);
		DrawDebugLine(GetWorld(), FocalLoc, WeaponLoc, FColor::Yellow, false, 60.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), CamLoc, FocalLoc, FColor::Blue, false, 60.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), WeaponLoc, FinalCamLoc, FColor::Red, false, 60.0f, 0, 2.0f);
	}
#endif
	return FTransform(CamRot, FinalCamLoc);
}

void UN1GameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits)
{
	UN1RangedWeaponInstance* WeaponData = InputData.WeaponData;
	check(WeaponData);

	const int32 BulletsPerCartridge = WeaponData->GetBulletsPerCartridge();

	for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
	{
		const float BaseSpreadAngle = WeaponData->GetCalculatedSpreadAngle();
		const float SpreadAngleMultiplier = WeaponData->GetCalculatedSpreadAngleMultiplier();
		const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;

		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);

		const FVector BulletDir = VRandConeNormalDistribution(InputData.AimDir, HalfSpreadAngleInRadians, WeaponData->GetSpreadExponent());

		const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->GetMaxDamageRange());
		FVector HitLocation = EndTrace;

		TArray<FHitResult> AllImpacts;

		FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, EndTrace, WeaponData->GetBulletTraceSweepRadius(), /*bIsSimulated=*/ false, /*out*/ AllImpacts);

		const AActor* HitActor = Impact.GetActor();

		if (HitActor)
		{
#if ENABLE_DRAW_DEBUG
			if (N1ConsoleVariables::DrawBulletHitDuration > 0.0f)
			{
				DrawDebugPoint(GetWorld(), Impact.ImpactPoint, N1ConsoleVariables::DrawBulletHitRadius, FColor::Red, false, N1ConsoleVariables::DrawBulletHitRadius);
			}
#endif

			if (AllImpacts.Num() > 0)
			{
				OutHits.Append(AllImpacts);
			}

			HitLocation = Impact.ImpactPoint;
		}

		// Make sure there's always an entry in OutHits so the direction can be used for tracers, etc...
		if (OutHits.Num() == 0)
		{
			if (!Impact.bBlockingHit)
			{
				// Locate the fake 'impact' at the end of the trace
				Impact.Location = EndTrace;
				Impact.ImpactPoint = EndTrace;
			}

			OutHits.Add(Impact);
		}
	}
}

FHitResult UN1GameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const
{
	FHitResult Impact;
	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		Impact = WeaponTrace(StartTrace, EndTrace, 0.0f, bIsSimulated, OutHits);
	}

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, SweepHits);
			const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);
			if (SweepHits.IsValidIndex(FirstPawnIdx))
			{
				bool bUseSweepHits = true;
				for (int32 Idx = 0; Idx < FirstPawnIdx; ++Idx)
				{
					const FHitResult& CurHitResult = SweepHits[Idx];
					auto Pred = [&CurHitResult](const FHitResult& Other)
						{
							return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
						};
					if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
					{
						bUseSweepHits = false;
						break;
					}
				}
				if (bUseSweepHits)
				{
					OutHits = SweepHits;
				}
			}
		}
	}
	return Impact;
}

int32 UN1GameplayAbility_RangedWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Idx;
		}
		else
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();
			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) &&
				(Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				return Idx;
			}
		}
	}
	return INDEX_NONE;
}

FHitResult UN1GameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;

	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;
	AddAdditionalTraceIgnoreActors(TraceParams);
	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);
	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel,
			FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}
	FHitResult Hit(ForceInit);
	if (HitResults.Num() > 0)
	{
		for (FHitResult& CurHitResult : HitResults)
		{
			auto Pred = [&CurHitResult](const FHitResult& Other)
				{
					return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
				};
			if (!OutHitResults.ContainsByPredicate(Pred))
			{
				OutHitResults.Add(CurHitResult);
			}
		}
		Hit = OutHitResults.Last();
	}
	else
	{
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}
	return Hit;
}

void UN1GameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;
		Avatar->GetAttachedActors(AttachedActors);
		TraceParams.AddIgnoredActors(AttachedActors);
	}
}

ECollisionChannel UN1GameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const
{
	return N1_TraceChannel_Weapon;

}

void UN1GameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilitySystemComponent);
	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FGameplayAbilityTargetDataHandle
			LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));
		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			OnRangeWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			K2_EndAbility();
		}
	}
}
