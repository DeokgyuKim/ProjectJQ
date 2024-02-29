// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"

#include "../Component/StatControlComponent.h"
#include "../Component/DecoratorComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JQComboActionData.h"
#include "TimerManager.h"

ACharacterBase::ACharacterBase()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	StatControlComponent = CreateDefaultSubobject<UStatControlComponent>("StatControl");
	DecoratorComponent = CreateDefaultSubobject<UDecoratorComponent>("Decorator");
}

void ACharacterBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ACharacterBase::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(MOVE_None); // 이동 할 수 없는 상태로 set

	// Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ACharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage); // 몽타주가 종료될 때 실행되는 콜백 등록 및 실행

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ACharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0); // 0인지 체크
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ACharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if(ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ACharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void ACharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if(HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);//문자열 조합 (AttackActon + n)하여 몽타주 섹션 재생
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void ACharacterBase::ProcessComboCommand()
{
	if(CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}
	if(!ComboTimerHandle.IsValid()) // 이미 타이머 발동, 더 이상 진행할 필요 없는 경우
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}
