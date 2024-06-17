// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/N1CameraAssistInterface.h"
#include "CommonPlayerController.h"
#include "Teams/N1TeamAgentInterface.h"

#include "N1PlayerController.generated.h"

struct FGenericTeamId;

class AN1HUD;
class AN1PlayerState;
class APawn;
class APlayerState;
class FPrimitiveComponentId;
class IInputInterface;
class UN1AbilitySystemComponent;
class UN1SettingsShared;
class UObject;
class UPlayer;
struct FFrame;

/**
 * AN1PlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class N1_API AN1PlayerController : public ACommonPlayerController, public IN1CameraAssistInterface, public IN1TeamAgentInterface
{
	GENERATED_BODY()

	public:

	AN1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "N1|PlayerController")
	AN1PlayerState* GetN1PlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "N1|PlayerController")
	UN1AbilitySystemComponent* GetN1AbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "N1|PlayerController")
	AN1HUD* GetN1HUD() const;

	// Call from game state logic to start recording an automatic client replay if ShouldRecordClientReplay returns true
	UFUNCTION(BlueprintCallable, Category = "N1|PlayerController")
	bool TryToRecordClientReplay();

	// Call to see if we should record a replay, subclasses could change this
	virtual bool ShouldRecordClientReplay();

	// Run a cheat command on the server.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheat(const FString& Msg);

	// Run a cheat command on the server for all players.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheatAll(const FString& Msg);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of AActor interface

	//~AController interface
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetPlayer(UPlayer* InPlayer) override;
	virtual void AddCheats(bool bForce) override;
	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

	//~IN1CameraAssistInterface interface
	virtual void OnCameraPenetratingTarget() override;
	//~End of IN1CameraAssistInterface interface

	//~IN1TeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnN1TeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IN1TeamAgentInterface interface

	UFUNCTION(BlueprintCallable, Category = "N1|Character")
	void SetIsAutoRunning(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "N1|Character")
	bool GetIsAutoRunning() const;

private:
	UPROPERTY()
	FOnN1TeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

private:
	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

private:
	void BroadcastOnPlayerStateChanged();

protected:

	//~APlayerController interface

	//~End of APlayerController interface

	void OnSettingsChanged(UN1SettingsShared* Settings);

	void OnStartAutoRun();
	void OnEndAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEndAutoRun"))
	void K2_OnEndAutoRun();

	bool bHideViewTargetPawnNextFrame = false;
};


// A player controller used for replay capture and playback
UCLASS()
class AN1ReplayPlayerController : public AN1PlayerController
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;
	virtual void SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds) override;
	virtual bool ShouldRecordClientReplay() override;

	// Callback for when the game state's RecorderPlayerState gets replicated during replay playback
	void RecorderPlayerStateUpdated(APlayerState* NewRecorderPlayerState);

	// Callback for when the followed player state changes pawn
	UFUNCTION()
	void OnPlayerStatePawnSet(APlayerState* ChangedPlayerState, APawn* NewPlayerPawn, APawn* OldPlayerPawn);

	// The player state we are currently following */
	UPROPERTY(Transient)
	TObjectPtr<APlayerState> FollowedPlayerState;
};
