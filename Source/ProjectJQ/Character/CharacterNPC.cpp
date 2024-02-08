// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterNPC.h"

ACharacterNPC::ACharacterNPC()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ACharacterNPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
