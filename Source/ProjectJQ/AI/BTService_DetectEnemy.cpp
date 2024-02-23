// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectEnemy.h"

#include "AIController.h"
#include "../Character/CharacterBase.h"
#include "../Component/StatControlComponent.h"
#include "../Core/MonsterAIController.h"

#include <BehaviorTree/BlackboardComponent.h>

#include "DrawDebugHelpers.h"

UBTService_DetectEnemy::UBTService_DetectEnemy()
{
	NodeName = TEXT("DetectEnemy");
	Interval = 0.5f;
}

void UBTService_DetectEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACharacterBase* characterBase = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(characterBase == nullptr)
		return;

	UWorld* world =characterBase->GetWorld();
	if(world == nullptr)
		return;

	FVector center = characterBase->GetActorLocation();
	UActorComponent* actorComponent = characterBase->GetComponentByClass(UStatControlComponent::StaticClass());
	if(actorComponent == nullptr)
		return;

	UStatControlComponent* statControl = Cast<UStatControlComponent>(actorComponent);
	if(statControl == nullptr)
		return;

	double range = statControl->GetStat(EStatControlType::DetectRange);

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams param;
	param.AddIgnoredActor(characterBase);

	if(world->OverlapMultiByChannel(overlapResults, center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(range), param))
	{
		TArray<ECharacterType> EnemyType;
		if(characterBase->GetCharacterType() == ECharacterType::Player)
			EnemyType.Add(ECharacterType::Monster);
		else if(characterBase->GetCharacterType() == ECharacterType::Monster)
			EnemyType.Add(ECharacterType::Player);

		if(EnemyType.IsEmpty())
		{
			DrawDebugSphere(world, center, range, 16, FColor::Red, false, 0.2f);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, nullptr);
			return;
		}

		TArray<TWeakObjectPtr<ACharacterBase>> targets;
		
		for(const FOverlapResult& result : overlapResults)
		{
			ACharacterBase* hitBase = Cast<ACharacterBase>(result.GetActor());
			if(hitBase == nullptr)
				continue;

			if(EnemyType.Find(hitBase->GetCharacterType()) == INDEX_NONE)
				continue;

			targets.Add(hitBase);

		}

		if(targets.IsEmpty())
		{
			DrawDebugSphere(world, center, range, 16, FColor::Red, false, 0.2f);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, nullptr);
			return;
		}

		FVector ownerLoc = characterBase->GetActorLocation();
		
		targets.Sort([&ownerLoc](TWeakObjectPtr<ACharacterBase> InActorlhs, TWeakObjectPtr<ACharacterBase> InActorrhs)
		{
			return FVector::Distance(InActorlhs->GetActorLocation(), ownerLoc) < FVector::Distance(InActorrhs->GetActorLocation(), ownerLoc);
		});
		
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, targets[0].Get());
#if ENABLE_DRAW_DEBUG
		DrawDebugSphere(world, center, range, 16, FColor::Green, false, 0.2f);
		DrawDebugLine(world, characterBase->GetActorLocation(), targets[0]->GetActorLocation(), FColor::Blue, false, 0.2f);
#endif
	}
	else
	{
#if ENABLE_DRAW_DEBUG
		DrawDebugSphere(world, center, range, 16, FColor::Red, false, 0.2f);
#endif
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, nullptr);
	}
	
}
