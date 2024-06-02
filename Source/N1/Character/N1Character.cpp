// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1Character.h"

#include "Camera/CameraComponent.h"
#include "Character/N1CharacterMovementComponent.h"
#include "Character/N1HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/N1PawnExtensionComponent.h"
#include "Player/N1PlayerState.h"
#include "Camera/N1CameraComponent.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "N1GameplayTags.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AN1Character

AN1Character::AN1Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UN1CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	PawnExtComponent = CreateDefaultSubobject<UN1PawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	CameraComponent = CreateDefaultSubobject<UN1CameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

	// Health Component
	HealthComponent = CreateDefaultSubobject<UN1HealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	// Character Movement Component
	UN1CharacterMovementComponent* N1MoveComp = CastChecked<UN1CharacterMovementComponent>(GetCharacterMovement());
	N1MoveComp->GravityScale = 1.0f;
	N1MoveComp->MaxAcceleration = 2400.0f;
	N1MoveComp->BrakingFrictionFactor = 1.0f;
	N1MoveComp->BrakingFriction = 6.0f;
	N1MoveComp->GroundFriction = 8.0f;
	N1MoveComp->BrakingDecelerationWalking = 1400.0f;
	N1MoveComp->bUseControllerDesiredRotation = false;
	N1MoveComp->bOrientRotationToMovement = false;
	N1MoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	N1MoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	N1MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	N1MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	N1MoveComp->SetCrouchedHalfHeight(65.0f);
}

UN1AbilitySystemComponent* AN1Character::GetN1AbilitySystemComponent() const
{
	return Cast<UN1AbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AN1Character::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetN1AbilitySystemComponent();
}

void AN1Character::ToggleCrouch()
{
	const UN1CharacterMovementComponent* N1MoveComp = CastChecked<UN1CharacterMovementComponent>(GetCharacterMovement());

	if (bIsCrouched || N1MoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (N1MoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

void AN1Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AN1Character::OnAbilitySystemInitialized()
{
	UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent();
	check(N1ASC);

	HealthComponent->InitializeWithAbilitySystem(N1ASC);

	InitializeGameplayTags();
}
void AN1Character::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
}
void AN1Character::PreInitializeComponents()
{
	Super::PreInitializeComponents();

}

void AN1Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AN1Character::OnRep_Owner()
{
	Super::OnRep_Owner();
}

void AN1Character::PostNetInit()
{
	Super::PostNetInit();
}

void AN1Character::InitializeGameplayTags()
{
	// Clear tags that may be lingering on the ability system from the previous pawn.
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		for (const TPair<uint8, FGameplayTag>& TagMapping : N1GameplayTags::MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				N1ASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : N1GameplayTags::CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				N1ASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UN1CharacterMovementComponent* N1MoveComp = CastChecked<UN1CharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(N1MoveComp->MovementMode, N1MoveComp->CustomMovementMode, true);
	}
}

void AN1Character::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UN1CharacterMovementComponent* MoveComp = CastChecked<UN1CharacterMovementComponent>(GetCharacterMovement());
	MoveComp->StopMovementImmediately();
	MoveComp->DisableMovement();
}

void AN1Character::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void AN1Character::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (UN1AbilitySystemComponent* ASC = GetN1AbilitySystemComponent())
	{
		if (ASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void AN1Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UN1CharacterMovementComponent* N1MoveComp = CastChecked<UN1CharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(N1MoveComp->MovementMode, N1MoveComp->CustomMovementMode, true);
}

void AN1Character::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = N1GameplayTags::CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = N1GameplayTags::MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			N1ASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void AN1Character::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		N1ASC->SetLooseGameplayTagCount(N1GameplayTags::Status_Crouching, 1);
	}


	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AN1Character::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		N1ASC->SetLooseGameplayTagCount(N1GameplayTags::Status_Crouching, 0);
	}

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool AN1Character::CanJumpInternal_Implementation() const
{
	return false;
}

void AN1Character::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void AN1Character::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

//////////////////////////////////////////////////////////////////////////
// Input
void AN1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}