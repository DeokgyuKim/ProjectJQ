// Fill out your copyright notice in the Description page of Project Settings.


#include "CpBTTaskAttack.h"
#include "..//Character/CharacterMonster.h"
#include "..//Character/CharacterPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..//Core/MonsterAIController.h"

#include "../Common/Macro.h"

UCpBTTaskAttack::UCpBTTaskAttack()
{
}

EBTNodeResult::Type UCpBTTaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACharacterMonster* rontrollingMonster = Cast<ACharacterMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == rontrollingMonster)
	{
		return EBTNodeResult::Failed;
	}

	ACharacterPC* Target = Cast<ACharacterPC>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	if (FVector::Distance(rontrollingMonster->GetActorLocation(), Target->GetActorLocation()) * 0.5f > rontrollingMonster->GetAttackRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("Can Attack"), false);
		return EBTNodeResult::Failed;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Monster Atatck"));

	return EBTNodeResult::Succeeded;
}