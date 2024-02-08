// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "CharacterBase.h"
#include "CharacterNPC.generated.h"

UCLASS(Blueprintable)
class ACharacterNPC : public ACharacterBase
{
	GENERATED_BODY()

public:
	ACharacterNPC();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

private:
};

