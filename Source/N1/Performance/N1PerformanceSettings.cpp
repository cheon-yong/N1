// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1PerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"
#include "Performance/N1PerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1PerformanceSettings)

//////////////////////////////////////////////////////////////////////

UN1PlatformSpecificRenderingSettings::UN1PlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UN1PlatformSpecificRenderingSettings* UN1PlatformSpecificRenderingSettings::Get()
{
	UN1PlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

UN1PerformanceSettings::UN1PerformanceSettings()
{
	PerPlatformSettings.Initialize(UN1PlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FN1PerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (EN1DisplayablePerformanceStat PerfStat : TEnumRange<EN1DisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}

