// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStampComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "ProjectJQ/Actor/JQProjectile.h"
#include "ProjectJQ/Character/CharacterPC.h"

// Sets default values for this component's properties
USkillStampComponent::USkillStampComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillStampComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerPC = Cast<ACharacterPC>(GetOwner());

	for(FSkillAnimMontageInfo skillInfo : SkillAnimInfos)
		EventSkillsMap.FindOrAdd(skillInfo.PlayTiming) = skillInfo;
	
}


// Called every frame
void USkillStampComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillStampComponent::ActivateSkill(ETriggerEvent InEvent, EAttackRangeType InAttackRange)
{
	_ActiveSkill(InEvent, InAttackRange);
}

void USkillStampComponent::_ActiveSkill(ETriggerEvent InEvent, EAttackRangeType InAttackRange)
{
	switch (InAttackRange)
	{
	case EAttackRangeType::Box:
		ActiveBoxCollisionAttack(InEvent);
		break;
	case EAttackRangeType::Sphere:
		ActiveSphereCollsionAttack(InEvent);
		break;
	case EAttackRangeType::Projectile:
		ActiveProjectileAttack(InEvent);
		break;
	case EAttackRangeType::None:
		break;
	}
}

void USkillStampComponent::ActiveBoxCollisionAttack(ETriggerEvent InEvent)
{
	//실제 작동 함수
	FCollisionQueryParams param;
	param.AddIgnoredActor(OwnerPC.Get());
	TArray<FOverlapResult> overlapResults;

	bool hitDetected = false;

	FVector pos = OwnerPC->GetActorLocation() + OwnerPC->GetActorForwardVector() * (OwnerPC->GetCapsuleComponent()->GetScaledCapsuleRadius() + ColliderBoxExtend.X / 2);
	FQuat rot = OwnerPC->GetActorForwardVector().ToOrientationQuat();
	
	if(GetWorld()->OverlapMultiByChannel(overlapResults, pos, rot, CCHANNEL_CharacterBase,
		FCollisionShape::MakeBox(ColliderBoxExtend / 2), param))
	{
		for(const FOverlapResult& result : overlapResults)
		{
			ACharacterBase* target = Cast<ACharacterBase>(result.GetActor());
			if(target == nullptr)
				continue;

			if(target->GetCharacterType() == TargetType)
			{
				FDamageEvent event;
				target->TakeDamage(10.f, event, OwnerPC->GetController(), OwnerPC.Get());
			}
		}
	}

	// 애니메이션 콤보 길이를 얻고 모듈러 연산하여 반복되게합니다.(ex. 0 -> 1 -> 2 -> 0 -> 1...)
	FSkillAnimMontageInfo* animInfo = EventSkillsMap.Find(InEvent);
	if(animInfo) Count = (Count + 1) % animInfo->ComboCount;

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = hitDetected ? FColor::Green : FColor::Red;
	DrawDebugBox(GetWorld(), pos, ColliderBoxExtend / 2, rot, DrawColor, false, 5.0f);
#endif
}

void USkillStampComponent::ActiveSphereCollsionAttack(ETriggerEvent InEvent)
{
	return;
}

void USkillStampComponent::ActiveProjectileAttack(ETriggerEvent InEvent)
{
	FVector pos = OwnerPC->GetActorLocation() + OwnerPC->GetActorForwardVector() * OwnerPC->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FRotator rot = FRotator::ZeroRotator;
	AJQProjectile* ProjectileActor = GetWorld()->SpawnActor<AJQProjectile>(ProjectileObject, pos, rot);
	if(ProjectileActor)
	{
		ProjectileActor->Initialize(OwnerPC->GetActorForwardVector(), OwnerPC->GetController());
	}
}

void USkillStampComponent::SkillTriggered()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillTriggerd"), *GetName());
	

	FSkillAnimMontageInfo* animInfo = EventSkillsMap.Find(ETriggerEvent::Triggered);
	
	if(animInfo == nullptr) return;
	if(!OwnerPC.IsValid()) return;
	if(Count > animInfo->ComboCount) return;
	
	FString inputSectionName = animInfo->SectionName.ToString();
	FName NextSection = *FString::Printf(TEXT("%s%d"), *inputSectionName, Count);
	OwnerPC->PlayCharacterAnimMontage(1.f, NextSection);
}

void USkillStampComponent::SkillStarted()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillStarted"), *GetName());

	for(const FSkillAnimMontageInfo& info : SkillAnimInfos)
		if(OwnerPC.IsValid() && info.PlayTiming == ETriggerEvent::Started)
			OwnerPC->PlayCharacterAnimMontage(1.f, info.SectionName);
}

void USkillStampComponent::SkillOnGoing()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillOnGoing"), *GetName());

	for(const FSkillAnimMontageInfo& info : SkillAnimInfos)
		if(OwnerPC.IsValid() && info.PlayTiming == ETriggerEvent::Ongoing)
			OwnerPC->PlayCharacterAnimMontage(1.f, info.SectionName);
}

void USkillStampComponent::SkillCanceled()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillCanceled"), *GetName());

	for(const FSkillAnimMontageInfo& info : SkillAnimInfos)
		if(OwnerPC.IsValid() && info.PlayTiming == ETriggerEvent::Canceled)
			OwnerPC->PlayCharacterAnimMontage(1.f, info.SectionName);
}

void USkillStampComponent::SkillCompleted()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillCompleted"), *GetName());

	for(const FSkillAnimMontageInfo& info : SkillAnimInfos)
		if(OwnerPC.IsValid() && info.PlayTiming == ETriggerEvent::Completed)
			OwnerPC->PlayCharacterAnimMontage(1.f, info.SectionName);
}

