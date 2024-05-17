// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "CharacterNPC.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "CharacterMonster.generated.h"

DECLARE_DELEGATE(FMonsterAttackFinished);

class AAIController;
UCLASS(Blueprintable)
class ACharacterMonster : public ACharacterNPC
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TWeakObjectPtr<AAIController> AIController;

	// ���� �ɷ�
	UPROPERTY()
	TMap<FName, TObjectPtr<class USkillStampComponent>> SkillStampComponents;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "JQSD_Range", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.f;

	// ������ �÷��̾�
	UPROPERTY()
	TWeakObjectPtr<ACharacterBase> TargetCharacter = nullptr;

	// ���� ����� ȣ�� �� ��������Ʈ
	FMonsterAttackFinished OnAttackFinished;
public:
	ACharacterMonster();

public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void SetAIController(TWeakObjectPtr<AAIController> InAIController);
	void OnFindRepeatTimer();

	void SetTarget(TWeakObjectPtr<ACharacterBase> InPCTarget) { TargetCharacter = InPCTarget; }
	float GetAttackRange() const { return AttackRange; }

	void BindAttackFinishDelegate(const FMonsterAttackFinished& InDelegate);

	virtual EPathFollowingRequestResult::Type MoveToTarget(APawn* InTarget, float InRadius, bool InOverlapStop = false);

	UFUNCTION()
	void Attack(const FName& InSkillComponentKey);

	UFUNCTION()
	virtual void AttackFinish();
};

