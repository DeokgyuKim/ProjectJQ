// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CpBTTaskAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UCpBTTaskAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCpBTTaskAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
