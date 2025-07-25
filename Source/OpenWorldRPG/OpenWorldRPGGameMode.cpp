// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenWorldRPGGameMode.h"
#include "OpenWorldRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HUD/RPGHUD.h"

AOpenWorldRPGGameMode::AOpenWorldRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    
    // Set the HUD class to use your custom RPGHUD
    HUDClass = URPGHUD::StaticClass();
}
