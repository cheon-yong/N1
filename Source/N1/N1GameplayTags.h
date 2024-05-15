// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

/**
 * 
 */
namespace N1GameplayTags
{
	N1_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	N1_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	N1_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	N1_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
}
