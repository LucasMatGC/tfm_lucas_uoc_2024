// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogueLike_ProjectGameMode.h"
#include "RogueLike_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARogueLike_ProjectGameMode::ARogueLike_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
