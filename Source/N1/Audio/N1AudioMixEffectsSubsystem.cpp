// Copyright Epic Games, Inc. All Rights Reserved.


#include "Audio/N1AudioMixEffectsSubsystem.h"

#include "AudioMixerBlueprintLibrary.h"
#include "AudioModulationStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "LoadingScreenManager.h"
#include "N1AudioSettings.h"
#include "Settings/N1SettingsLocal.h"
#include "Sound/SoundEffectSubmix.h"
#include "SoundControlBus.h"
#include "SoundControlBusMix.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1AudioMixEffectsSubsystem)

class FSubsystemCollectionBase;

void UN1AudioMixEffectsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UN1AudioMixEffectsSubsystem::Deinitialize()
{
	if (ULoadingScreenManager* LoadingScreenManager = UGameInstance::GetSubsystem<ULoadingScreenManager>(GetWorld()->GetGameInstance()))
	{
		LoadingScreenManager->OnLoadingScreenVisibilityChangedDelegate().RemoveAll(this);
		ApplyOrRemoveLoadingScreenMix(false);
	}

	Super::Deinitialize();
}

bool UN1AudioMixEffectsSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool bShouldCreateSubsystem = Super::ShouldCreateSubsystem(Outer);

	if (Outer)
	{
		if (UWorld* World = Outer->GetWorld())
		{
			bShouldCreateSubsystem = DoesSupportWorldType(World->WorldType) && bShouldCreateSubsystem;
		}
	}

	return bShouldCreateSubsystem;
}

void UN1AudioMixEffectsSubsystem::PostInitialize()
{
	if (const UN1AudioSettings* N1AudioSettings = GetDefault<UN1AudioSettings>())
	{
		if (UObject* ObjPath = N1AudioSettings->DefaultControlBusMix.TryLoad())
		{
			if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(ObjPath))
			{
				DefaultBaseMix = SoundControlBusMix;
			}
			else
			{
				ensureMsgf(SoundControlBusMix, TEXT("Default Control Bus Mix reference missing from N1 Audio Settings."));
			}
		}

		if (UObject* ObjPath = N1AudioSettings->LoadingScreenControlBusMix.TryLoad())
		{
			if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(ObjPath))
			{
				LoadingScreenMix = SoundControlBusMix;
			}
			else
			{
				ensureMsgf(SoundControlBusMix, TEXT("Loading Screen Control Bus Mix reference missing from N1 Audio Settings."));
			}
		}

		if (UObject* ObjPath = N1AudioSettings->UserSettingsControlBusMix.TryLoad())
		{
			if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(ObjPath))
			{
				UserMix = SoundControlBusMix;
			}
			else
			{
				ensureMsgf(SoundControlBusMix, TEXT("User Control Bus Mix reference missing from N1 Audio Settings."));
			}
		}

		if (UObject* ObjPath = N1AudioSettings->OverallVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				OverallControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("Overall Control Bus reference missing from N1 Audio Settings."));
			}
		}

		if (UObject* ObjPath = N1AudioSettings->MusicVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				MusicControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("Music Control Bus reference missing from N1 Audio Settings."));
			}
		}

		if (UObject* ObjPath = N1AudioSettings->SoundFXVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				SoundFXControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("SoundFX Control Bus reference missing from N1 Audio Settings."));
			}
		}

		if (UObject* ObjPath = N1AudioSettings->DialogueVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				DialogueControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("Dialogue Control Bus reference missing from N1 Audio Settings."));
			}
		}

		if (UObject* ObjPath = N1AudioSettings->VoiceChatVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				VoiceChatControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("VoiceChat Control Bus reference missing from N1 Audio Settings."));
			}
		}

		// Load HDR Submix Effect Chain
		for (const FN1SubmixEffectChainMap& SoftSubmixEffectChain : N1AudioSettings->HDRAudioSubmixEffectChain)
		{
			FN1AudioSubmixEffectsChain NewEffectChain;

			if (UObject* SubmixObjPath = SoftSubmixEffectChain.Submix.LoadSynchronous())
			{
				if (USoundSubmix* Submix = Cast<USoundSubmix>(SubmixObjPath))
				{
					NewEffectChain.Submix = Submix;
					TArray<USoundEffectSubmixPreset*> NewPresetChain;

					for (const TSoftObjectPtr<USoundEffectSubmixPreset>& SoftEffect : SoftSubmixEffectChain.SubmixEffectChain)
					{
						if (UObject* EffectObjPath = SoftEffect.LoadSynchronous())
						{
							if (USoundEffectSubmixPreset* SubmixPreset = Cast<USoundEffectSubmixPreset>(EffectObjPath))
							{
								NewPresetChain.Add(SubmixPreset);
							}
						}
					}

					NewEffectChain.SubmixEffectChain.Append(NewPresetChain);
				}
			}

			HDRSubmixEffectChain.Add(NewEffectChain);
		}

		// Load LDR Submix Effect Chain
		for (const FN1SubmixEffectChainMap& SoftSubmixEffectChain : N1AudioSettings->LDRAudioSubmixEffectChain)
		{
			FN1AudioSubmixEffectsChain NewEffectChain;

			if (UObject* SubmixObjPath = SoftSubmixEffectChain.Submix.LoadSynchronous())
			{
				if (USoundSubmix* Submix = Cast<USoundSubmix>(SubmixObjPath))
				{
					NewEffectChain.Submix = Submix;
					TArray<USoundEffectSubmixPreset*> NewPresetChain;

					for (const TSoftObjectPtr<USoundEffectSubmixPreset>& SoftEffect : SoftSubmixEffectChain.SubmixEffectChain)
					{
						if (UObject* EffectObjPath = SoftEffect.LoadSynchronous())
						{
							if (USoundEffectSubmixPreset* SubmixPreset = Cast<USoundEffectSubmixPreset>(EffectObjPath))
							{
								NewPresetChain.Add(SubmixPreset);
							}
						}
					}

					NewEffectChain.SubmixEffectChain.Append(NewPresetChain);
				}
			}

			LDRSubmixEffectChain.Add(NewEffectChain);
		}
	}

	// Register with the loading screen manager
	if (ULoadingScreenManager* LoadingScreenManager = UGameInstance::GetSubsystem<ULoadingScreenManager>(GetWorld()->GetGameInstance()))
	{
		LoadingScreenManager->OnLoadingScreenVisibilityChangedDelegate().AddUObject(this, &ThisClass::OnLoadingScreenStatusChanged);
		ApplyOrRemoveLoadingScreenMix(LoadingScreenManager->GetLoadingScreenDisplayStatus());
	}
}

void UN1AudioMixEffectsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	if (const UWorld* World = InWorld.GetWorld())
	{
		// Activate the default base mix
		if (DefaultBaseMix)
		{
			UAudioModulationStatics::ActivateBusMix(World, DefaultBaseMix);
		}

		// Retrieve the user settings
		if (const UN1SettingsLocal* N1SettingsLocal = GetDefault<UN1SettingsLocal>())
		{
			// Activate the User Mix
			if (UserMix)
			{
				UAudioModulationStatics::ActivateBusMix(World, UserMix);

				if (OverallControlBus && MusicControlBus && SoundFXControlBus && DialogueControlBus && VoiceChatControlBus)
				{
					const FSoundControlBusMixStage OverallControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, OverallControlBus, N1SettingsLocal->GetOverallVolume());
					const FSoundControlBusMixStage MusicControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, MusicControlBus, N1SettingsLocal->GetMusicVolume());
					const FSoundControlBusMixStage SoundFXControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, SoundFXControlBus, N1SettingsLocal->GetSoundFXVolume());
					const FSoundControlBusMixStage DialogueControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, DialogueControlBus, N1SettingsLocal->GetDialogueVolume());
					const FSoundControlBusMixStage VoiceChatControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, VoiceChatControlBus, N1SettingsLocal->GetVoiceChatVolume());

					TArray<FSoundControlBusMixStage> ControlBusMixStageArray;
					ControlBusMixStageArray.Add(OverallControlBusMixStage);
					ControlBusMixStageArray.Add(MusicControlBusMixStage);
					ControlBusMixStageArray.Add(SoundFXControlBusMixStage);
					ControlBusMixStageArray.Add(DialogueControlBusMixStage);
					ControlBusMixStageArray.Add(VoiceChatControlBusMixStage);

					UAudioModulationStatics::UpdateMix(World, UserMix, ControlBusMixStageArray);
				}
			}

			ApplyDynamicRangeEffectsChains(N1SettingsLocal->IsHDRAudioModeEnabled());
		}
	}
}

void UN1AudioMixEffectsSubsystem::ApplyDynamicRangeEffectsChains(bool bHDRAudio)
{
	TArray<FN1AudioSubmixEffectsChain> AudioSubmixEffectsChainToApply;
	TArray<FN1AudioSubmixEffectsChain> AudioSubmixEffectsChainToClear;

	// If HDR Audio is selected, then we clear out any existing LDR Submix Effect Chain Overrides
	// otherwise the reverse is the case.
	if (bHDRAudio)
	{
		AudioSubmixEffectsChainToApply.Append(HDRSubmixEffectChain);
		AudioSubmixEffectsChainToClear.Append(LDRSubmixEffectChain);
	}
	else
	{
		AudioSubmixEffectsChainToApply.Append(LDRSubmixEffectChain);
		AudioSubmixEffectsChainToClear.Append(HDRSubmixEffectChain);
	}

	// We want to collect just the submixes we need to actually clear, otherwise they'll be overridden by the new settings
	TArray<USoundSubmix*> SubmixesLeftToClear;

	// We want to get the submixes that are not being overridden by the new effect chains, so we can clear those out separately
	for (const FN1AudioSubmixEffectsChain& EffectChainToClear : AudioSubmixEffectsChainToClear)
	{
		bool bAddToList = true;

		for (const FN1AudioSubmixEffectsChain& SubmixEffectChain : AudioSubmixEffectsChainToApply)
		{
			if (SubmixEffectChain.Submix == EffectChainToClear.Submix)
			{
				bAddToList = false;

				break;
			}
		}

		if (bAddToList)
		{
			SubmixesLeftToClear.Add(EffectChainToClear.Submix);
		}
	}


	// Override submixes
	for (const FN1AudioSubmixEffectsChain& SubmixEffectChain : AudioSubmixEffectsChainToApply)
	{
		if (SubmixEffectChain.Submix)
		{
			UAudioMixerBlueprintLibrary::SetSubmixEffectChainOverride(GetWorld(), SubmixEffectChain.Submix, SubmixEffectChain.SubmixEffectChain, 0.1f);

		}
	}

	// Clear remaining submixes
	for (USoundSubmix* Submix : SubmixesLeftToClear)
	{
		UAudioMixerBlueprintLibrary::ClearSubmixEffectChainOverride(GetWorld(), Submix, 0.1f);
	}
}

void UN1AudioMixEffectsSubsystem::OnLoadingScreenStatusChanged(bool bShowingLoadingScreen)
{
	ApplyOrRemoveLoadingScreenMix(bShowingLoadingScreen);
}

void UN1AudioMixEffectsSubsystem::ApplyOrRemoveLoadingScreenMix(bool bWantsLoadingScreenMix)
{
	UWorld* World = GetWorld();

	if (bAppliedLoadingScreenMix != bWantsLoadingScreenMix && LoadingScreenMix && World)
	{
		if (bWantsLoadingScreenMix)
		{
			// Apply the mix
			UAudioModulationStatics::ActivateBusMix(World, LoadingScreenMix);
		}
		else
		{
			// Remove the mix
			UAudioModulationStatics::DeactivateBusMix(World, LoadingScreenMix);
		}
		bAppliedLoadingScreenMix = bWantsLoadingScreenMix;
	}
}

bool UN1AudioMixEffectsSubsystem::DoesSupportWorldType(const EWorldType::Type World) const
{
	// We only need this subsystem on Game worlds (PIE included)
	return (World == EWorldType::Game || World == EWorldType::PIE);
}

