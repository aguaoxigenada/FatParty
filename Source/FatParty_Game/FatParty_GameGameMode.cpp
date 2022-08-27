// Copyright Epic Games, Inc. All Rights Reserved.

#include "FatParty_GameGameMode.h"
#include "FatParty_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFatParty_GameGameMode::AFatParty_GameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
