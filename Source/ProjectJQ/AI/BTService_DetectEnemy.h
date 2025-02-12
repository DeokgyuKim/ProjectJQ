// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UBTService_DetectEnemy : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_DetectEnemy();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
