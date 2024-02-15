// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"

#include "../Component/StatControlComponent.h"
#include "../Component/DecoratorComponent.h"

ACharacterBase::ACharacterBase()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	StatControlComponent = CreateDefaultSubobject<UStatControlComponent>("StatControl");
	DecoratorComponent = CreateDefaultSubobject<UDecoratorComponent>("Decorator");
}

void ACharacterBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
