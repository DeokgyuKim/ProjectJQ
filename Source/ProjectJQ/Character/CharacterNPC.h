// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "CharacterBase.h"
#include "CharacterNPC.generated.h"

class UJQWidgetComponent;

UCLASS(Blueprintable)
class ACharacterNPC : public ACharacterBase
{
	GENERATED_BODY()

protected:
	int32 HpBarId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_UI")
	TObjectPtr<UJQWidgetComponent> WidgetComponent;

public:
	ACharacterNPC();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

private:
};

