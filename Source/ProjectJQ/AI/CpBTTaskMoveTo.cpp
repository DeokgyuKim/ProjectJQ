// Fill out your copyright notice in the Description page of Project Settings.


#include "CpBTTaskMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..//Core/MonsterAIController.h"
#include "..//Character/CharacterMonster.h"
#include "..//Component/StatControlComponent.h"
#include "Navigation/PathFollowingComponent.h"

UCpBTTaskMoveTo::UCpBTTaskMoveTo()
{
}

EBTNodeResult::Type UCpBTTaskMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacterMonster* monster = Cast<ACharacterMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == monster)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterAIController* aiController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (nullptr == aiController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if (nullptr == player)
	{
		return EBTNodeResult::Failed;
	}

	FVector monsterPos = monster->GetActorLocation();
	FVector playerPos = player->GetActorLocation();

	float attackRange = monster->GetAttackRange();

	float length = FVector::Distance(monsterPos, playerPos);
	if (length * 0.5f < attackRange)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Can Attack"), true);
		return EBTNodeResult::Failed;
	}

	EPathFollowingRequestResult::Type result = monster->MoveToTarget(player, attackRange, true);
	if (result == EPathFollowingRequestResult::Type::Failed)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Can Attack"), false);
	return EBTNodeResult::Succeeded;
}
