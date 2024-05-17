// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterMonster.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TimerManager.h"

#include "..//Component/SkillStampComponent.h"
#include "..//Component/StatControlComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "..//Animation/CpMonsterAnimInstance.h"
#include "Animation/AnimMontage.h"

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

    // AnimInstance
    static ConstructorHelpers::FClassFinder<UAnimInstance> animinstanceclassref(TEXT("/Game/ProjectJQ/Blueprints/Animation/ABP_NPC_Super.ABP_NPC_Super_C"));
    if (animinstanceclassref.Class)
    {
        GetMesh()->SetAnimInstanceClass(animinstanceclassref.Class);
    }

    // AnimMontage
    static ConstructorHelpers::FObjectFinder<UAnimMontage> attack_Montage(
        TEXT
        ("/Game/ProjectJQ/Blueprints/Animation/AM_MonsterAttack.AM_MonsterAttack")
    );
    if (attack_Montage.Succeeded())
    {
        AnimMontage = attack_Montage.Object;
    }

#pragma region Attack SkillStampCompnent
    if (USkillStampComponent* comp = CreateDefaultSubobject<USkillStampComponent>("AttackCompnent"))
    {
        SkillStampComponents.Emplace("Attack", comp);

        comp->TargetType = ECharacterType::Player;
        comp->ColliderBoxExtend = FVector::ZeroVector;
        comp->AttackRangeType = EAttackRangeType::Box;

        //comp->SkillAnimInfos;
        //comp->EventSkillsMap

        FBoxInfo boxInfo;
        boxInfo.AttackBox = FVector(50.f);
        comp->BasicAttackInfo = boxInfo;
    }
#pragma endregion


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
    // 지금은 사용중이 아닙니다
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

void ACharacterMonster::BindAttackFinishDelegate(const FMonsterAttackFinished& InDelegate)
{
    OnAttackFinished = InDelegate;
}

EPathFollowingRequestResult::Type ACharacterMonster::MoveToTarget(APawn* InTarget, float InRadius, bool InOverlapStop)
{
    EPathFollowingRequestResult::Type result = AIController->MoveToActor(InTarget, InRadius, InOverlapStop);

    return result;
}

void ACharacterMonster::Attack(const FName& InSkillComponentKey)
{
    if (USkillStampComponent* comp = *SkillStampComponents.Find(InSkillComponentKey))
    {
        if (nullptr == AnimMontage)
            return;

        if (GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == AnimMontage)
        {
            if(GetMesh()->GetAnimInstance()->Montage_IsActive(AnimMontage))
                return;
        }

        float duration = PlayAnimMontage(AnimMontage, 1.0f);

        // 애니메이션을 끝나는 시점 호출
         FOnMontageEnded attackEnd;
         attackEnd.BindUFunction(this, TEXT("AttackFinish"));
         GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(attackEnd, AnimMontage);


         comp->Activate();
    }
}

void ACharacterMonster::AttackFinish()
{
    // Attack 종료시 델리게이트호출
    OnAttackFinished.ExecuteIfBound();

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Monster Atatck Finish"));
}
