// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterMonster.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TimerManager.h"

#include "AIController.h"


ACharacterMonster::ACharacterMonster()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CharacterType = ECharacterType::Monster;

	//캡슐의 프로파일 이름 설정
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_JQCAPSULE);

	//매시는 충돌에서 제외한다.
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

void ACharacterMonster::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ACharacterMonster::SetAIController(TWeakObjectPtr<AAIController> InAIController)
{
	if(InAIController == nullptr)
		return;
	AIController = InAIController;
}

void ACharacterMonster::OnFindRepeatTimer()
{
    return;

    if (!AIController.IsValid())
        return;

    APawn* pawn = AIController->GetPawn();
    if (nullptr == pawn)
        return;

    // 월드상 네비메쉬를 얻어옴
    UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    if (nullptr == navSystem)
        return;

    FNavLocation nextLocation;
    if (navSystem->GetRandomPointInNavigableRadius(pawn->GetActorLocation(), 300.f, nextLocation))
    {
        // 도착지점 이동
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController.Get(), nextLocation.Location);
        UE_LOG(LogTemp, Warning, TEXT("Next Loction : %s"), *nextLocation.Location.ToString());
    }
}