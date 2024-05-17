// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectJQGameMode.h"
#include "ProjectCpPlayerController.h"
#include "../Character/CharacterPC.h"
#include "UObject/ConstructorHelpers.h"

AProjectJQGameMode::AProjectJQGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectCpPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ProjectJQ/Blueprints/Character/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/ProjectJQ/Blueprints/Core/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}