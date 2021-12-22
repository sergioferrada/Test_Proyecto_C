// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProyectoCGameMode.h"
#include "ProyectoCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProyectoCGameMode::AProyectoCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
