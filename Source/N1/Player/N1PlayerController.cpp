// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1PlayerController.h"
#include "CommonInputTypeEnum.h"
#include "Components/PrimitiveComponent.h"
#include "N1LogChannels.h"
#include "N1PlayerState.h"
#include "Camera/N1PlayerCameraManager.h"
#include "UI/N1HUD.h"
#include "AbilitySystem/N1AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "N1GameplayTags.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameInstance.h"
#include "AbilitySystemGlobals.h"
#include "CommonInputSubsystem.h"
#include "N1LocalPlayer.h"
#include "Game/N1GameState.h"
#include "Settings/N1SettingsLocal.h"
#include "Settings/N1SettingsShared.h"
#include "ReplaySubsystem.h"
#include "Development/N1DeveloperSettings.h"
#include "GameMapsSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1PlayerController)

namespace N1
{
	namespace Input
	{
		static int32 ShouldAlwaysPlayForceFeedback = 0;
		static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(TEXT("N1PC.ShouldAlwaysPlayForceFeedback"),
			ShouldAlwaysPlayForceFeedback,
			TEXT("Should force feedback effects be played, even if the last input device was not a gamepad?"));
	}
}

AN1PlayerController::AN1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AN1PlayerCameraManager::StaticClass();
}

void AN1PlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AN1PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void AN1PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AN1PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Disable replicating the PC target view as it doesn't work well for replays or client-side spectating.
	// The engine TargetViewRotation is only set in APlayerController::TickActor if the server knows ahead of time that 
	// a specific pawn is being spectated and it only replicates down for COND_OwnerOnly.
	// In client-saved replays, COND_OwnerOnly is never true and the target pawn is not always known at the time of recording.
	// To support client-saved replays, the replication of this was moved to ReplicatedViewRotation and updated in PlayerTick.
	DISABLE_REPLICATED_PROPERTY(APlayerController, TargetViewRotation);
}

void AN1PlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void AN1PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// If we are auto running then add some player input
	if (GetIsAutoRunning())
	{
		if (APawn* CurrentPawn = GetPawn())
		{
			const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			CurrentPawn->AddMovementInput(MovementDirection, 1.0f);
		}
	}

	AN1PlayerState* N1PlayerState = GetN1PlayerState();

	if (PlayerCameraManager && N1PlayerState)
	{
		APawn* TargetPawn = PlayerCameraManager->GetViewTargetPawn();

		if (TargetPawn)
		{
			// Update view rotation on the server so it replicates
			if (HasAuthority() || TargetPawn->IsLocallyControlled())
			{
				N1PlayerState->SetReplicatedViewRotation(TargetPawn->GetViewRotation());
			}

			// Update the target view rotation if the pawn isn't locally controlled
			if (!TargetPawn->IsLocallyControlled())
			{
				N1PlayerState = TargetPawn->GetPlayerState<AN1PlayerState>();
				if (N1PlayerState)
				{
					// Get it from the spectated pawn's player state, which may not be the same as the PC's playerstate
					TargetViewRotation = N1PlayerState->GetReplicatedViewRotation();
				}
			}
		}
	}
}

AN1PlayerState* AN1PlayerController::GetN1PlayerState() const
{
	return CastChecked<AN1PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UN1AbilitySystemComponent* AN1PlayerController::GetN1AbilitySystemComponent() const
{
	const AN1PlayerState* N1PS = GetN1PlayerState();
	return (N1PS ? N1PS->GetN1AbilitySystemComponent() : nullptr);
}

AN1HUD* AN1PlayerController::GetN1HUD() const
{
	return CastChecked<AN1HUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

bool AN1PlayerController::TryToRecordClientReplay()
{
	// See if we should record a replay
	if (ShouldRecordClientReplay())
	{
		//if (UN1ReplaySubsystem* ReplaySubsystem = GetGameInstance()->GetSubsystem<UN1ReplaySubsystem>())
		//{
		//	APlayerController* FirstLocalPlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		//	if (FirstLocalPlayerController == this)
		//	{
		//		// If this is the first player, update the spectator player for local replays and then record
		//		if (AN1GameState* GameState = Cast<AN1GameState>(GetWorld()->GetGameState()))
		//		{
		//			GameState->SetRecorderPlayerState(PlayerState);

		//			ReplaySubsystem->RecordClientReplay(this);
		//			return true;
		//		}
		//	}
		//}
	}
	return false;
}

bool AN1PlayerController::ShouldRecordClientReplay()
{
//	UWorld* World = GetWorld();
//	UGameInstance* GameInstance = GetGameInstance();
//	if (GameInstance != nullptr &&
//		World != nullptr &&
//		!World->IsPlayingReplay() &&
//		!World->IsRecordingClientReplay() &&
//		NM_DedicatedServer != GetNetMode() &&
//		IsLocalPlayerController())
//	{
//		FString DefaultMap = UGameMapsSettings::GetGameDefaultMap();
//		FString CurrentMap = World->URL.Map;
//
//#if WITH_EDITOR
//		CurrentMap = UWorld::StripPIEPrefixFromPackageName(CurrentMap, World->StreamingLevelsPrefix);
//#endif
//		if (CurrentMap == DefaultMap)
//		{
//			// Never record demos on the default frontend map, this could be replaced with a better check for being in the main menu
//			return false;
//		}
//
//		if (UReplaySubsystem* ReplaySubsystem = GameInstance->GetSubsystem<UReplaySubsystem>())
//		{
//			if (ReplaySubsystem->IsRecording() || ReplaySubsystem->IsPlaying())
//			{
//				// Only one at a time
//				return false;
//			}
//		}
//
//		// If this is possible, now check the settings
//		if (const UN1LocalPlayer* N1LocalPlayer = Cast<UN1LocalPlayer>(GetLocalPlayer()))
//		{
//			if (N1LocalPlayer->GetLocalSettings()->ShouldAutoRecordReplays())
//			{
//				return true;
//			}
//		}
//	}
	return false;
}

void AN1PlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}

void AN1PlayerController::OnPlayerStateChanged()
{
	// Empty, place for derived classes to implement without having to hook all the other events
}

void AN1PlayerController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();

	// Unbind from the old player state, if any
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (IN1TeamAgentInterface* PlayerStateTeamInterface = Cast<IN1TeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (IN1TeamAgentInterface* PlayerStateTeamInterface = Cast<IN1TeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	// Broadcast the team change (if it really has)
	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	LastSeenPlayerState = PlayerState;
}

void AN1PlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AN1PlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AN1PlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void AN1PlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);

	if (const UN1LocalPlayer* N1LocalPlayer = Cast<UN1LocalPlayer>(InPlayer))
	{
		UN1SettingsShared* UserSettings = N1LocalPlayer->GetSharedSettings();
		UserSettings->OnSettingChanged.AddUObject(this, &ThisClass::OnSettingsChanged);

		OnSettingsChanged(UserSettings);
	}
}

void AN1PlayerController::OnSettingsChanged(UN1SettingsShared* InSettings)
{
	bForceFeedbackEnabled = InSettings->GetForceFeedbackEnabled();
}

void AN1PlayerController::AddCheats(bool bForce)
{
}

void AN1PlayerController::ServerCheat_Implementation(const FString& Msg)
{
}

bool AN1PlayerController::ServerCheat_Validate(const FString& Msg)
{
	return true;
}

void AN1PlayerController::ServerCheatAll_Implementation(const FString& Msg)
{
}

bool AN1PlayerController::ServerCheatAll_Validate(const FString& Msg)
{
	return true;
}

void AN1PlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AN1PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		N1ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AN1PlayerController::OnCameraPenetratingTarget()
{
	bHideViewTargetPawnNextFrame = true;
}

void AN1PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#if WITH_SERVER_CODE && WITH_EDITOR
	if (GIsEditor && (InPawn != nullptr) && (GetPawn() == InPawn))
	{
		for (const FN1CheatToRun& CheatRow : GetDefault<UN1DeveloperSettings>()->CheatsToRun)
		{
			if (CheatRow.Phase == ECheatExecutionTime::OnPlayerPawnPossession)
			{
				ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
			}
		}
	}
#endif

	SetIsAutoRunning(false);
}

void AN1PlayerController::SetIsAutoRunning(const bool bEnabled)
{
	const bool bIsAutoRunning = GetIsAutoRunning();
	if (bEnabled != bIsAutoRunning)
	{
		if (!bEnabled)
		{
			OnEndAutoRun();
		}
		else
		{
			OnStartAutoRun();
		}
	}
}

bool AN1PlayerController::GetIsAutoRunning() const
{
	bool bIsAutoRunning = false;
	if (const UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		bIsAutoRunning = N1ASC->GetTagCount(N1GameplayTags::Status_AutoRunning) > 0;
	}
	return bIsAutoRunning;
}

void AN1PlayerController::OnStartAutoRun()
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		N1ASC->SetLooseGameplayTagCount(N1GameplayTags::Status_AutoRunning, 1);
		K2_OnStartAutoRun();
	}
}

void AN1PlayerController::OnEndAutoRun()
{
	if (UN1AbilitySystemComponent* N1ASC = GetN1AbilitySystemComponent())
	{
		N1ASC->SetLooseGameplayTagCount(N1GameplayTags::Status_AutoRunning, 0);
		K2_OnEndAutoRun();
	}
}

void AN1PlayerController::UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId)
{
	if (bForceFeedbackEnabled)
	{
		if (const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(GetLocalPlayer()))
		{
			const ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
			if (N1::Input::ShouldAlwaysPlayForceFeedback || CurrentInputType == ECommonInputType::Gamepad || CurrentInputType == ECommonInputType::Touch)
			{
				InputInterface->SetForceFeedbackChannelValues(ControllerId, ForceFeedbackValues);
				return;
			}
		}
	}

	InputInterface->SetForceFeedbackChannelValues(ControllerId, FForceFeedbackValues());
}

void AN1PlayerController::UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents)
{
	Super::UpdateHiddenComponents(ViewLocation, OutHiddenComponents);

	if (bHideViewTargetPawnNextFrame)
	{
		AActor* const ViewTargetPawn = PlayerCameraManager ? Cast<AActor>(PlayerCameraManager->GetViewTarget()) : nullptr;
		if (ViewTargetPawn)
		{
			// internal helper func to hide all the components
			auto AddToHiddenComponents = [&OutHiddenComponents](const TInlineComponentArray<UPrimitiveComponent*>& InComponents)
				{
					// add every component and all attached children
					for (UPrimitiveComponent* Comp : InComponents)
					{
						if (Comp->IsRegistered())
						{
							OutHiddenComponents.Add(Comp->GetPrimitiveSceneId());

							for (USceneComponent* AttachedChild : Comp->GetAttachChildren())
							{
								static FName NAME_NoParentAutoHide(TEXT("NoParentAutoHide"));
								UPrimitiveComponent* AttachChildPC = Cast<UPrimitiveComponent>(AttachedChild);
								if (AttachChildPC && AttachChildPC->IsRegistered() && !AttachChildPC->ComponentTags.Contains(NAME_NoParentAutoHide))
								{
									OutHiddenComponents.Add(AttachChildPC->GetPrimitiveSceneId());
								}
							}
						}
					}
				};

			//TODO Solve with an interface.  Gather hidden components or something.
			//TODO Hiding isn't awesome, sometimes you want the effect of a fade out over a proximity, needs to bubble up to designers.

			// hide pawn's components
			TInlineComponentArray<UPrimitiveComponent*> PawnComponents;
			ViewTargetPawn->GetComponents(PawnComponents);
			AddToHiddenComponents(PawnComponents);

			//// hide weapon too
			//if (ViewTargetPawn->CurrentWeapon)
			//{
			//	TInlineComponentArray<UPrimitiveComponent*> WeaponComponents;
			//	ViewTargetPawn->CurrentWeapon->GetComponents(WeaponComponents);
			//	AddToHiddenComponents(WeaponComponents);
			//}
		}

		// we consumed it, reset for next frame
		bHideViewTargetPawnNextFrame = false;
	}
}

void AN1PlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogN1Teams, Error, TEXT("You can't set the team ID on a player controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
}

FGenericTeamId AN1PlayerController::GetGenericTeamId() const
{
	if (const IN1TeamAgentInterface* PSWithTeamInterface = Cast<IN1TeamAgentInterface>(PlayerState))
	{
		return PSWithTeamInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

FOnN1TeamIndexChangedDelegate* AN1PlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AN1PlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}

//////////////////////////////////////////////////////////////////////
// AN1ReplayPlayerController

void AN1ReplayPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// The state may go invalid at any time due to scrubbing during a replay
	if (!IsValid(FollowedPlayerState))
	{
		UWorld* World = GetWorld();

		// Listen for changes for both recording and playback
		if (AN1GameState* GameState = Cast<AN1GameState>(World->GetGameState()))
		{
			if (!GameState->OnRecorderPlayerStateChangedEvent.IsBoundToObject(this))
			{
				GameState->OnRecorderPlayerStateChangedEvent.AddUObject(this, &ThisClass::RecorderPlayerStateUpdated);
			}
			if (APlayerState* RecorderState = GameState->GetRecorderPlayerState())
			{
				RecorderPlayerStateUpdated(RecorderState);
			}
		}
	}
}

void AN1ReplayPlayerController::SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds)
{
	// Default behavior is to interpolate to TargetViewRotation which is set from APlayerController::TickActor but it's not very smooth

	Super::SmoothTargetViewRotation(TargetPawn, DeltaSeconds);
}

bool AN1ReplayPlayerController::ShouldRecordClientReplay()
{
	return false;
}

void AN1ReplayPlayerController::RecorderPlayerStateUpdated(APlayerState* NewRecorderPlayerState)
{
	if (NewRecorderPlayerState)
	{
		FollowedPlayerState = NewRecorderPlayerState;

		// Bind to when pawn changes and call now
		NewRecorderPlayerState->OnPawnSet.AddUniqueDynamic(this, &AN1ReplayPlayerController::OnPlayerStatePawnSet);
		OnPlayerStatePawnSet(NewRecorderPlayerState, NewRecorderPlayerState->GetPawn(), nullptr);
	}
}

void AN1ReplayPlayerController::OnPlayerStatePawnSet(APlayerState* ChangedPlayerState, APawn* NewPlayerPawn, APawn* OldPlayerPawn)
{
	if (ChangedPlayerState == FollowedPlayerState)
	{
		SetViewTarget(NewPlayerPawn);
	}
}

