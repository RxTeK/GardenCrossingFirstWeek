// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_Garden/Public/MyProject8GameMode.h"
#include "UObject/ConstructorHelpers.h"

AMyProject8GameMode::AMyProject8GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
