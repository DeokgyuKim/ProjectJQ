// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "CharacterNPC.h"
#include "CharacterMonster.generated.h"

class AAIController;
UCLASS(Blueprintable)
class ACharacterMonster : public ACharacterNPC
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TWeakObjectPtr<AAIController> AIController;

	// 몬스터 능력
	UPROPERTY()
	TMap<FName, TObjectPtr<class USkillStampComponent>> SkillStampComponents;

	// 공격 범위
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "JQSD_Range", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.f;

	// 추적할 플레이어
	UPROPERTY()
	TWeakObjectPtr<ACharacterBase> TargetCharacter = nullptr;
public:
	ACharacterMonster();

public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void SetAIController(TWeakObjectPtr<AAIController> InAIController);
	void OnFindRepeatTimer();

	void SetTarget(TWeakObjectPtr<ACharacterBase> InPCTarget) { TargetCharacter = InPCTarget; }
	float GetAttackRange() const { return AttackRange; }

	virtual void MoveTo();
};

