// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterMonster.h"

ACharacterMonster::ACharacterMonster()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CharacterType = ECharacterType::Monster;
}

void ACharacterMonster::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ACharacterMonster::SetAIController(TWeakObjectPtr<AAIController> InAIController)
{
	if(InAIController == nullptr)
		return;
	AIController = InAIController;
}
