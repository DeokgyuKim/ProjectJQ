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

	// ���� �ɷ�
	UPROPERTY()
	TMap<FName, TObjectPtr<class USkillStampComponent>> SkillStampComponents;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "JQSD_Range", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 50.f;

	// ������ �÷��̾�
	UPROPERTY()
	TWeakObjectPtr<APawn> TargetPC = nullptr;
public:
	ACharacterMonster();

public:
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void SetAIController(TWeakObjectPtr<AAIController> InAIController);
	void OnFindRepeatTimer();

	void SetTarget(TWeakObjectPtr<ACharacterBase> InPCTarget) { TargetPC = InPCTarget; }
};

