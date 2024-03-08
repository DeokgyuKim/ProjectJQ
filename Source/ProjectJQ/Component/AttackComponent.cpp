// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectJQ/Character/CharacterBase.h"
#include "ProjectJQ/Character/CharacterMonster.h"
#include "ProjectJQ/Character/CharacterPC.h"
#include "ProjectJQ/Character/JQComboActionData.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::ComboActionBegin() // Idle 상태에서 공격 상태로 변환 됐을 때
{
	ACharacterBase* Base = Cast<ACharacterBase>(GetOwner());
	if(!Base) return;
	
	// Combo Status
	CurrentCombo = 1;
	
	Base->GetCharacterMovement()->SetMovementMode(MOVE_None); // 이동 할 수 없는 상태로 set

	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = Base->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UAttackComponent::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
	
}

void UAttackComponent::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded) // 공격이 끝났을 때
{
	ACharacterBase* Base = Cast<ACharacterBase>(GetOwner());
	if(!Base) return;
	
	ensure(CurrentCombo != 0); // 0인지 체크
	CurrentCombo = 0;
	
	Base->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
}

void UAttackComponent::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if(ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UAttackComponent::ComboCheck, ComboEffectiveTime, false);
	}
}

void UAttackComponent::ComboCheck()
{
	ACharacterBase* Base = Cast<ACharacterBase>(GetOwner());
	
	if(!Base) return;
	
	ComboTimerHandle.Invalidate();
	if(HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = Base->GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);//문자열 조합 (AttackActon + n)하여 몽타주 섹션 재생
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void UAttackComponent::ProcessComboCommand()
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

void UAttackComponent::AttackHitCheck()
{
	ACharacterPC* PC = Cast<ACharacterPC>(GetOwner());
	if(!PC) return;
	
	TArray<FHitResult> OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, PC);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = PC->GetActorLocation() + PC->GetActorForwardVector() * PC->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + PC->GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_JQACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	if(HitDetected)
	{
		for(auto It = OutHitResult.CreateIterator(); It ; ++It)
		{
			ACharacterBase* Monster = Cast<ACharacterBase>(It->GetActor());
			if(Monster)
			{
				FDamageEvent DamageEvent;
				Monster->TakeDamage(AttackDamage, DamageEvent, PC->GetController(), PC);
			}
		}
	}

	//전처리기 작성 하기
#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(PC->GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
	
}

