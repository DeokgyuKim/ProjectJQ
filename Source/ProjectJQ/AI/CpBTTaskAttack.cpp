// Fill out your copyright notice in the Description page of Project Settings.


#include "CpBTTaskAttack.h"
#include "..//Character/CharacterMonster.h"
#include "..//Character/CharacterPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..//Core/MonsterAIController.h"
#include "..//Component/SkillStampComponent.h"

#include "../Common/Macro.h"

UCpBTTaskAttack::UCpBTTaskAttack()
{
}

EBTNodeResult::Type UCpBTTaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACharacterMonster* monster = Cast<ACharacterMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == monster)
	{
		return EBTNodeResult::Failed;
	}

	ACharacterPC* target = Cast<ACharacterPC>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == target)
	{
		return EBTNodeResult::Failed;
	}

	if (FVector::Distance(monster->GetActorLocation(), target->GetActorLocation()) * 0.5f > monster->GetAttackRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Can Attack"), false);
		return EBTNodeResult::Failed;
	}

	FMonsterAttackFinished onAttackFinished;
	onAttackFinished.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	monster->BindAttackFinishDelegate(onAttackFinished);

	OwnerComp.GetAIOwner()->StopMovement();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Monster Atatck"));
	monster->Attack("Attack");

	return EBTNodeResult::InProgress;
}
