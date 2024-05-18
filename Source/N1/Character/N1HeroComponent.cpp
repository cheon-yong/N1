// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/N1HeroComponent.h"
#include "N1PawnData.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/GameFrameworkComponent.h"
#include "Camera/N1CameraComponent.h"
#include "N1PawnExtensionComponent.h"
#include "N1LogChannels.h"
#include "N1GameplayTags.h"
#include "Player/N1PlayerState.h"
#include <Player/N1PlayerController.h>
#include <EnhancedInputSubsystems.h>
#include <Input/N1EnhancedInputComponent.h>
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

const FName UN1HeroComponent::NAME_ActorFeatureName("Hero");

UN1HeroComponent::UN1HeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UN1HeroComponent::OnRegister()
{
	Super::OnRegister();
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogN1, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}
	RegisterInitStateFeature();
}

void UN1HeroComponent::BeginPlay()
{
	Super::BeginPlay();
	BindOnActorInitStateChanged(UN1PawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);
	ensure(TryToChangeInitState(N1GameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UN1HeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UN1HeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UN1PawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == N1GameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UN1HeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	APawn* Pawn = GetPawn<APawn>();
	AN1PlayerState* PS = GetPlayerState<AN1PlayerState>();
	if (!CurrentState.IsValid() && DesiredState == N1GameplayTags::InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == N1GameplayTags::InitState_Spawned && DesiredState == N1GameplayTags::InitState_DataAvailable)
	{
		if (!PS)
		{
			return false;
		}

		return true;
	}
	if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		return PS && Manager->HasFeatureReachedInitState(Pawn,
			UN1PawnExtensionComponent::NAME_ActorFeatureName, N1GameplayTags::InitState_DataInitialized);
	}

	if (CurrentState == N1GameplayTags::InitState_DataInitialized && DesiredState == N1GameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UN1HeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AN1PlayerState* PS = GetPlayerState<AN1PlayerState>();
		if (!ensure(Pawn && PS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UN1PawnData* PawnData = nullptr;
		if (UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UN1PawnData>();
		}
		if (bIsLocallyControlled && PawnData)
		{
			if (UN1CameraComponent* CameraComponent = UN1CameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		if (AN1PlayerController* N1PC = GetController<AN1PlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UN1HeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {
		N1GameplayTags::InitState_Spawned,
		N1GameplayTags::InitState_DataAvailable,
		N1GameplayTags::InitState_DataInitialized,
		N1GameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

TSubclassOf<UN1CameraMode> UN1HeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}
	if (UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UN1PawnData* PawnData = PawnExtComp->GetPawnData<UN1PawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}
	return nullptr;
}

void UN1HeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UN1PawnData* PawnData = PawnExtComp->GetPawnData<UN1PawnData>())
		{
			if (const UN1InputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}

							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}

				// The Lyra Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the ULyraInputComponent or modify this component accordingly.
				UN1EnhancedInputComponent* IC = Cast<UN1EnhancedInputComponent>(PlayerInputComponent);
				if (ensureMsgf(IC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULyraInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					IC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					
					IC->BindNativeAction(InputConfig, N1GameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					IC->BindNativeAction(InputConfig, N1GameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}
}

void UN1HeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		if (Value.X != 0.0f)
		{
			const FVector MovementDirection =
				MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}
		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection =
				MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}

}

void UN1HeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}
	if (Value.Y != 0.0f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}