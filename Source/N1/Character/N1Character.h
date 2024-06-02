// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModularCharacter.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"

#include "N1Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UN1PawnExtensionComponent;
class UN1CameraComponent;
class UN1AbilitySystemComponent;
class UN1HealthComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AN1Character : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AN1Character(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
			
	UFUNCTION(BlueprintCallable, Category = "N1|Character")
	UN1AbilitySystemComponent* GetN1AbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void ToggleCrouch();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay();

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;
	virtual void PostNetInit() override;
	void InitializeGameplayTags();


	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeathFinished"))
	void K2_OnDeathFinished();

	// About Death
	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();
	
	// About Move
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual bool CanJumpInternal_Implementation() const;
	
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "N1|Character")
	TObjectPtr<UN1PawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "N1|Character")
	TObjectPtr<UN1CameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "N1|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UN1HealthComponent> HealthComponent;

};

