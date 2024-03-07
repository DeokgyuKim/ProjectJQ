// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterMonster.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProjectJQ/Physics/JQCollision.h"

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
