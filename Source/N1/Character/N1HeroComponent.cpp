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
