// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAIController.h"
#include "TimerManager.h"
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

    
    ACharacterMonster* monster = Cast<ACharacterMonster>(InPawn);
    if (nullptr == monster)
        return;

    GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMonsterAIController::OnRepeatTimer, RepeatInterval, true);
}

void AMonsterAIController::OnUnPossess()
{
    Super::OnUnPossess();

    // 타이머 초기화
    GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AMonsterAIController::OnRepeatTimer()
{
    ACharacterMonster* monster = Cast<ACharacterMonster>(GetPawn());
    if (nullptr == monster)
        return;

    monster->OnFindRepeatTimer();
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

ACharacterMonster* AMonsterAIController::GetMonster()
{
    return Cast<ACharacterMonster>(GetPawn());
}
