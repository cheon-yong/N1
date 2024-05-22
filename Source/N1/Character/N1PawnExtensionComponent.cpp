#include "N1PawnExtensionComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/GameFrameworkComponentDelegates.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Character/N1PawnExtensionComponent.h"
#include "N1LogChannels.h"
#include "N1GameplayTags.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"

const FName UN1PawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UN1PawnExtensionComponent::UN1PawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UN1PawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
	if (!GetPawn<APawn>())
	{
		UE_LOG(LogN1, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
		return;
	}
	RegisterInitStateFeature();
}

void UN1PawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	ensure(TryToChangeInitState(N1GameplayTags::InitState_Spawned));
	CheckDefaultInitialization();

}

void UN1PawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UN1PawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == N1GameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UN1PawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == N1GameplayTags::InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == N1GameplayTags::InitState_Spawned && DesiredState == N1GameplayTags::InitState_DataAvailable)
	{
		if (!PawnData)
		{
			return false;
		}
		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bHasAuthority || bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}
	if (CurrentState == N1GameplayTags::InitState_DataAvailable && DesiredState == N1GameplayTags::InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(Pawn, N1GameplayTags::InitState_DataAvailable);
	}
	if (CurrentState == N1GameplayTags::InitState_DataInitialized && DesiredState == N1GameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UN1PawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = { 
		N1GameplayTags::InitState_Spawned, 
		N1GameplayTags::InitState_DataAvailable,
		N1GameplayTags::InitState_DataInitialized, 
		N1GameplayTags::InitState_GameplayReady 
	};

	ContinueInitStateChain(StateChain);
}

void UN1PawnExtensionComponent::SetPawnData(const UN1PawnData* InPawnData)
{
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	if (PawnData)
	{
		return;
	}
	PawnData = InPawnData;
	CheckDefaultInitialization();
}

void UN1PawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UN1PawnExtensionComponent::InitializeAbilitySystem(UN1AbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC && InOwnerActor);
	if (AbilitySystemComponent == InASC)
	{
		return;
	}
	if (AbilitySystemComponent)
	{
		UninitializeAbilitySystem();
	}
	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InASC->GetAvatarActor();
	check(!ExistingAvatar);
	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);
}

void UN1PawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent = nullptr;
}
