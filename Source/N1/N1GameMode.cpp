// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1GameMode.h"
#include "N1Character.h"
#include "UObject/ConstructorHelpers.h"

AN1GameMode::AN1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
