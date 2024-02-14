// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "../Interface/ObjectManagementTargetInterface.h"
#include "CharacterBase.generated.h"

class UStatControlComponent;

UCLASS(Blueprintable)
class ACharacterBase : public ACharacter, public IObjectManagementTargetInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatControlComponent> StatControlComponent;

	ECharacterType CharacterType = ECharacterType::None;
public:
	ACharacterBase();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

public:
	ECharacterType GetCharacterType() const {return CharacterType;}
};

