// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"

#include "N1GamePhaseSubsystem.generated.h"

template <typename T> class TSubclassOf;

class UN1GamePhaseAbility;
class UObject;
struct FFrame;
struct FGameplayAbilitySpecHandle;

DECLARE_DYNAMIC_DELEGATE_OneParam(FN1GamePhaseDynamicDelegate, const UN1GamePhaseAbility*, Phase);
DECLARE_DELEGATE_OneParam(FN1GamePhaseDelegate, const UN1GamePhaseAbility* Phase);

DECLARE_DYNAMIC_DELEGATE_OneParam(FN1GamePhaseTagDynamicDelegate, const FGameplayTag&, PhaseTag);
DECLARE_DELEGATE_OneParam(FN1GamePhaseTagDelegate, const FGameplayTag& PhaseTag);

// Match rule for message receivers
UENUM(BlueprintType)
enum class EPhaseTagMatchType : uint8
{
	// An exact match will only receive messages with exactly the same channel
	// (e.g., registering for "A.B" will match a broadcast of A.B but not A.B.C)
	ExactMatch,

	// A partial match will receive any messages rooted in the same channel
	// (e.g., registering for "A.B" will match a broadcast of A.B as well as A.B.C)
	PartialMatch
};


/** Subsystem for managing N1's game phases using gameplay tags in a nested manner, which allows parent and child 
 * phases to be active at the same time, but not sibling phases.
 * Example:  Game.Playing and Game.Playing.WarmUp can coexist, but Game.Playing and Game.ShowingScore cannot. 
 * When a new phase is started, any active phases that are not ancestors will be ended.
 * Example: if Game.Playing and Game.Playing.CaptureTheFlag are active when Game.Playing.PostGame is started, 
 *     Game.Playing will remain active, while Game.Playing.CaptureTheFlag will end.
 */
UCLASS()
class UN1GamePhaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UN1GamePhaseSubsystem();

	virtual void PostInitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void StartPhase(TSubclassOf<UN1GamePhaseAbility> PhaseAbility, FN1GamePhaseDelegate PhaseEndedCallback = FN1GamePhaseDelegate());

	//TODO Return a handle so folks can delete these.  They will just grow until the world resets.
	//TODO Should we just occasionally clean these observers up?  It's not as if everyone will properly unhook them even if there is a handle.
	void WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FN1GamePhaseTagDelegate& WhenPhaseActive);
	void WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FN1GamePhaseTagDelegate& WhenPhaseEnd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure = false, meta = (AutoCreateRefTerm = "PhaseTag"))
	bool IsPhaseActive(const FGameplayTag& PhaseTag) const;

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName="Start Phase", AutoCreateRefTerm = "PhaseEnded"))
	void K2_StartPhase(TSubclassOf<UN1GamePhaseAbility> Phase, const FN1GamePhaseDynamicDelegate& PhaseEnded);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName = "When Phase Starts or Is Active", AutoCreateRefTerm = "WhenPhaseActive"))
	void K2_WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, FN1GamePhaseTagDynamicDelegate WhenPhaseActive);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName = "When Phase Ends", AutoCreateRefTerm = "WhenPhaseEnd"))
	void K2_WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, FN1GamePhaseTagDynamicDelegate WhenPhaseEnd);

	void OnBeginPhase(const UN1GamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);
	void OnEndPhase(const UN1GamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);

private:
	struct FN1GamePhaseEntry
	{
	public:
		FGameplayTag PhaseTag;
		FN1GamePhaseDelegate PhaseEndedCallback;
	};

	TMap<FGameplayAbilitySpecHandle, FN1GamePhaseEntry> ActivePhaseMap;

	struct FPhaseObserver
	{
	public:
		bool IsMatch(const FGameplayTag& ComparePhaseTag) const;
	
		FGameplayTag PhaseTag;
		EPhaseTagMatchType MatchType = EPhaseTagMatchType::ExactMatch;
		FN1GamePhaseTagDelegate PhaseCallback;
	};

	TArray<FPhaseObserver> PhaseStartObservers;
	TArray<FPhaseObserver> PhaseEndObservers;

	friend class UN1GamePhaseAbility;
};
