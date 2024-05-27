// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1Character.h"

#include "Camera/CameraComponent.h"
#include "Character/N1CharacterMovementComponent.h"
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

	CameraComponent = CreateDefaultSubobject<UN1CameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

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

void AN1Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AN1Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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

//////////////////////////////////////////////////////////////////////////
// Input

void AN1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}