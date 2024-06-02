// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1HeroComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Logging/MessageLog.h"
#include "Input/N1MappableConfigPair.h"
#include "N1LogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Player/N1PlayerController.h"
#include "Player/N1PlayerState.h"
#include "Player/N1LocalPlayer.h"
#include "Character/N1PawnExtensionComponent.h"
#include "Character/N1PawnData.h"
#include "Character/N1Character.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "Input/N1InputConfig.h"
#include "Input/N1EnhancedInputComponent.h"
#include "Camera/N1CameraComponent.h"
#include "N1GameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "PlayerMappableInputConfig.h"
#include "Camera/N1CameraMode.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1HeroComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace N1Hero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UN1HeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UN1HeroComponent::NAME_ActorFeatureName("Hero");

UN1HeroComponent::UN1HeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void UN1HeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogN1, Error, TEXT("[UN1HeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("N1HeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("N1HeroComponent");

			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));

			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UN1HeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == N1GameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == N1GameplayTags::InitState_Spawned && DesiredState == N1GameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AN1PlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AN1PlayerController* N1PC = GetController<AN1PlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !N1PC || !N1PC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AN1PlayerState* N1PS = GetPlayerState<AN1PlayerState>();

		return N1PS && Manager->HasFeatureReachedInitState(Pawn, UN1PawnExtensionComponent::NAME_ActorFeatureName, N1GameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == N1GameplayTags::InitState_DataInitialized && DesiredState == N1GameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UN1HeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AN1PlayerState* N1PS = GetPlayerState<AN1PlayerState>();
		if (!ensure(Pawn && N1PS))
		{
			return;
		}

		const UN1PawnData* PawnData = nullptr;

		if (UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UN1PawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(N1PS->GetN1AbilitySystemComponent(), N1PS);
		}

		if (AN1PlayerController* N1PC = GetController<AN1PlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (UN1CameraComponent* CameraComponent = UN1CameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UN1HeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UN1PawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == N1GameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UN1HeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { N1GameplayTags::InitState_Spawned, N1GameplayTags::InitState_DataAvailable, N1GameplayTags::InitState_DataInitialized, N1GameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UN1HeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UN1PawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(N1GameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UN1HeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
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

	const UN1LocalPlayer* LP = Cast<UN1LocalPlayer>(PC->GetLocalPlayer());
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

				// The N1 Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UN1EnhancedInputComponent or modify this component accordingly.
				UN1EnhancedInputComponent* N1IC = Cast<UN1EnhancedInputComponent>(PlayerInputComponent);
				if (ensureMsgf(N1IC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UN1EnhancedInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					N1IC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					N1IC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					N1IC->BindNativeAction(InputConfig, N1GameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					N1IC->BindNativeAction(InputConfig, N1GameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					N1IC->BindNativeAction(InputConfig, N1GameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					N1IC->BindNativeAction(InputConfig, N1GameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
					N1IC->BindNativeAction(InputConfig, N1GameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UN1HeroComponent::AddAdditionalInputConfig(const UN1InputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UN1EnhancedInputComponent* N1IC = Pawn->FindComponentByClass<UN1EnhancedInputComponent>();
		if (ensureMsgf(N1IC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UN1EnhancedInputComponent or a subclass of it.")))
		{
			N1IC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UN1HeroComponent::RemoveAdditionalInputConfig(const UN1InputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UN1HeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UN1HeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UN1AbilitySystemComponent* N1ASC = PawnExtComp->GetN1AbilitySystemComponent())
			{
				N1ASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void UN1HeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UN1PawnExtensionComponent* PawnExtComp = UN1PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UN1AbilitySystemComponent* N1ASC = PawnExtComp->GetN1AbilitySystemComponent())
		{
			N1ASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UN1HeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (AN1PlayerController* N1Controller = Cast<AN1PlayerController>(Controller))
	{
		N1Controller->SetIsAutoRunning(false);
	}

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
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
		Pawn->AddControllerPitchInput(-Value.Y);
	}
}

void UN1HeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * N1Hero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * N1Hero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UN1HeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AN1Character* Character = GetPawn<AN1Character>())
	{
		Character->ToggleCrouch();
	}
}

void UN1HeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (AN1PlayerController* Controller = Cast<AN1PlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}
	}
}

TSubclassOf<UN1CameraMode> UN1HeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

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

void UN1HeroComponent::OnInputConfigActivated(const FLoadedMappableConfigPair& ConfigPair)
{
}

void UN1HeroComponent::OnInputConfigDeactivated(const FLoadedMappableConfigPair& ConfigPair)
{
}

void UN1HeroComponent::SetAbilityCameraMode(TSubclassOf<UN1CameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UN1HeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

