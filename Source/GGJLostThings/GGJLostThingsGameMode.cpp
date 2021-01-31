// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJLostThingsGameMode.h"
#include "GGJLostThingsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGGJLostThingsGameMode::AGGJLostThingsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
