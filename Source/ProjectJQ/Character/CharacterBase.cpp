// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ACharacterBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
