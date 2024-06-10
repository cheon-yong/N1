// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1AimSensitivityData.h"

#include "Settings/N1SettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1AimSensitivityData)

UN1AimSensitivityData::UN1AimSensitivityData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SensitivityMap =
	{
		{ EN1GamepadSensitivity::Slow,			0.5f },
		{ EN1GamepadSensitivity::SlowPlus,		0.75f },
		{ EN1GamepadSensitivity::SlowPlusPlus,	0.9f },
		{ EN1GamepadSensitivity::Normal,		1.0f },
		{ EN1GamepadSensitivity::NormalPlus,	1.1f },
		{ EN1GamepadSensitivity::NormalPlusPlus,1.25f },
		{ EN1GamepadSensitivity::Fast,			1.5f },
		{ EN1GamepadSensitivity::FastPlus,		1.75f },
		{ EN1GamepadSensitivity::FastPlusPlus,	2.0f },
		{ EN1GamepadSensitivity::Insane,		2.5f },
	};
}

const float UN1AimSensitivityData::SensitivtyEnumToFloat(const EN1GamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}

