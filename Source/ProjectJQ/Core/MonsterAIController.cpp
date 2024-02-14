// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAIController.h"
#include "../Character//CharacterMonster.h"

const FName AMonsterAIController::TargetKey(TEXT("Target"));

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AMonsterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    RunAi(InPawn, false);
}

//Ai를 실행합니다.
void AMonsterAIController::RunAi(APawn* inPawn, bool inForced)
{
    if(inPawn == nullptr)
        return;

    Cast<ACharacterMonster>(inPawn)->SetAIController(this);
    
    if(AIBehavior != nullptr)
        RunBehaviorTree(AIBehavior);
}