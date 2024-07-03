// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"

#include "N1WeaponSpawner.generated.h"

namespace EEndPlayReason { enum Type : int; }

class APawn;
class UCapsuleComponent;
class UN1InventoryItemDefinition;
class UN1WeaponPickupDefinition;
class UObject;
class UPrimitiveComponent;
class UStaticMeshComponent;
struct FFrame;
struct FGameplayTag;
struct FHitResult;

UCLASS(Blueprintable,BlueprintType)
class N1_API AN1WeaponSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AN1WeaponSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

protected:
	//Data asset used to configure a Weapon Spawner
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "N1|WeaponPickup")
	TObjectPtr<UN1WeaponPickupDefinition> WeaponDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponAvailability, Category = "N1|WeaponPickup")
	bool bIsWeaponAvailable;

	//The amount of time between weapon pickup and weapon spawning in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|WeaponPickup")
	float CoolDownTime;

	//Delay between when the weapon is made available and when we check for a pawn standing in the spawner. Used to give the bIsWeaponAvailable OnRep time to fire and play FX. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|WeaponPickup")
	float CheckExistingOverlapDelay;

	//Used to drive weapon respawn time indicators 0-1
	UPROPERTY(BlueprintReadOnly, Transient, Category = "N1|WeaponPickup")
	float CoolDownPercentage;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|WeaponPickup")
	TObjectPtr<UCapsuleComponent> CollisionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "N1|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> PadMesh;

	UPROPERTY(BlueprintReadOnly, Category = "N1|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "N1|WeaponPickup")
	float WeaponMeshRotationSpeed;

	FTimerHandle CoolDownTimerHandle;

	FTimerHandle CheckOverlapsDelayTimerHandle;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	//Check for pawns standing on pad when the weapon is spawned. 
	void CheckForExistingOverlaps();

	UFUNCTION(BlueprintNativeEvent)
	void AttemptPickUpWeapon(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "N1|WeaponPickup")
	bool GiveWeapon(TSubclassOf<UN1InventoryItemDefinition> WeaponItemClass, APawn* ReceivingPawn);

	void StartCoolDown();

	UFUNCTION(BlueprintCallable, Category = "N1|WeaponPickup")
	void ResetCoolDown();

	UFUNCTION()
	void OnCoolDownTimerComplete();

	void SetWeaponPickupVisibility(bool bShouldBeVisible);

	UFUNCTION(BlueprintNativeEvent, Category = "N1|WeaponPickup")
	void PlayPickupEffects();

	UFUNCTION(BlueprintNativeEvent, Category = "N1|WeaponPickup")
	void PlayRespawnEffects();

	UFUNCTION()
	void OnRep_WeaponAvailability();

	/** Searches an item definition type for a matching stat and returns the value, or 0 if not found */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "N1|WeaponPickup")
	static int32 GetDefaultStatFromItemDef(const TSubclassOf<UN1InventoryItemDefinition> WeaponItemClass, FGameplayTag StatTag);
};
