// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "CharacterBase.h"
#include "CharacterMonster.generated.h"

class AAIController;
UCLASS(Blueprintable)
class ACharacterMonster : public ACharacterBase
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TWeakObjectPtr<AAIController> AIController;

public:
	ACharacterMonster();

public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void SetAIController(TWeakObjectPtr<AAIController> InAIController);
};

