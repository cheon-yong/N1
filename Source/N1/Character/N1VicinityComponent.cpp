// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/N1VicinityComponent.h"

#include "N1GameplayTags.h"
#include "Character/N1PawnExtensionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/N1PlayerController.h"
#include "Player/N1PlayerState.h"
#include "Character/N1Character.h"
#include "N1LogChannels.h"
#include "Engine/EngineTypes.h"

const FName UN1VicinityComponent::NAME_BindInputsNow("BindInputsNow");
const FName UN1VicinityComponent::NAME_ActorFeatureName("Vicinity");

UN1VicinityComponent::UN1VicinityComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

bool UN1VicinityComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
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

void UN1VicinityComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AN1PlayerState* N1PS = GetPlayerState<AN1PlayerState>();
		if (!ensure(Pawn && N1PS))
		{
			return;
		}

		// TODO : Intialize something
		
		if (AN1Character* N1Character = Cast<AN1Character>(Pawn))
		{
			N1Character->VicinityCollision->OnComponentBeginOverlap.AddDynamic(this, &UN1VicinityComponent::OnOverlapBegin);
		}
	}
}

void UN1VicinityComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
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

void UN1VicinityComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { N1GameplayTags::InitState_Spawned, N1GameplayTags::InitState_DataAvailable, N1GameplayTags::InitState_DataInitialized, N1GameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UN1VicinityComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	UE_LOG(LogN1, Log, TEXT("On Overlap"));
}

void UN1VicinityComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogN1, Error, TEXT("[UN1VicinityComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		/*if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("N1VicinityComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("N1VicinityComponent");

			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));

			FMessageLog(HeroMessageLogName).Open();
		}*/
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

void UN1VicinityComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UN1PawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(N1GameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UN1VicinityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}
