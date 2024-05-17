// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStampComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "../Actor/CpProjectile.h"
#include "../Character/CharacterPC.h"
#include "../SubSystem/ObjectManagementGSS.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	if(AttackRangeType == EAttackRangeType::None)
	{
		LOG_SCREEN(FColor::Red, TEXT("%s의 AttackRange가 None입니다."), *GetName())
		return;
	}
}


// Called every frame
void USkillStampComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillStampComponent::ActivateSkill(FName InAnimSectionName)
{
	_ActiveSkill(InAnimSectionName);
}

void USkillStampComponent::_ActiveSkill(FName InAnimSectionName)
{
	//현재 재생중인 애니메이션 섹션 네임으로 카운트를 올릴 FSkillAnimMontageInfo를 찾음.
	FSkillAnimMontageInfo* findInfo = nullptr;
	for(TPair<ETriggerEvent, FSkillAnimMontageInfo> animMontage : EventSkillsMap)
	{
		for(int i = 0; i < animMontage.Value.ComboCount; ++i)
		{
			FString inputSectionName = animMontage.Value.SectionName.ToString();
			FName findName = *FString::Printf(TEXT("%s%d"), *inputSectionName, Count);
			if(findName == InAnimSectionName)
			{
				findInfo = &animMontage.Value;
				break;
			}
		}
		if(findInfo)
			break;
	}

	if(findInfo == nullptr)
		return;
	
	switch (AttackRangeType)
	{
	case EAttackRangeType::Box:
		ActiveBoxCollisionAttack(findInfo);
		break;
	case EAttackRangeType::Sphere:
		ActiveSphereCollsionAttack(findInfo);
		break;
	case EAttackRangeType::Projectile:
		ActiveProjectileAttack(findInfo);
		break;
	case EAttackRangeType::Arc:
		break;
	case EAttackRangeType::None:
		break;
	}
	
	OwnerPC->SetCanAttack(true);
}

void USkillStampComponent::ActiveBoxCollisionAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo)
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
	FSkillAnimMontageInfo* animInfo = InCurrentPlayAnimMontageInfo;
	if(animInfo) Count = (Count + 1) % animInfo->ComboCount;

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = hitDetected ? FColor::Green : FColor::Red;
	DrawDebugBox(GetWorld(), pos, ColliderBoxExtend / 2, rot, DrawColor, false, 5.0f);
#endif
}

void USkillStampComponent::ActiveSphereCollsionAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo)
{
	FCollisionQueryParams param;
	param.AddIgnoredActor(OwnerPC.Get());
	TArray<FOverlapResult> overlapResults;
	
	
	if(GetWorld()->OverlapMultiByChannel(overlapResults, OwnerPC->GetActorLocation(), FQuat::Identity, CCHANNEL_CharacterBase,
		FCollisionShape::MakeSphere(SphereAttackInfo.SphereRadius), param))
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

	
}

void USkillStampComponent::ActiveProjectileAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo)
{
	FVector pos = OwnerPC->GetActorLocation() + OwnerPC->GetActorForwardVector() * OwnerPC->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FRotator rot = FRotator::ZeroRotator;

	UObjectManagementGSS* gss = GetOwner()->GetGameInstance()->GetSubsystem<UObjectManagementGSS>();

	FSpawnParam param;
	param.Location = pos;
	param.Rotation = rot;
	param.CallbackSpawn = nullptr;
	if(ProjectileAttackInfo.IsSingleShot)
	{
		ACpProjectile* ProjectileActor = gss->CreateActor<ACpProjectile>(ProjectileAttackInfo.ProjectileObject, param, GetOwner());
	
		if(ProjectileActor)
		{
			ProjectileActor->Initialize(OwnerPC->GetActorForwardVector(), OwnerPC->GetController(), ProjectileAttackInfo.Length);
		}
	}
	else
	{
		float angleBetweenProjectile = ProjectileAttackInfo.Angle / (ProjectileAttackInfo.NumOfProjectile - 1);
		FVector startVector = RotateVectorFromYaw(OwnerPC->GetActorForwardVector(), -ProjectileAttackInfo.Angle / 2);
		for(int i = 0; i < ProjectileAttackInfo.NumOfProjectile; ++i)
		{
			ACpProjectile* ProjectileActor = gss->CreateActor<ACpProjectile>(ProjectileAttackInfo.ProjectileObject, param, GetOwner());
	
			if(ProjectileActor)
			{
				ProjectileActor->Initialize(RotateVectorFromYaw(startVector, i * angleBetweenProjectile), OwnerPC->GetController(), ProjectileAttackInfo.Length);
			}
		}
	}
}

void USkillStampComponent::ActiveArcAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo)
{
	FCollisionQueryParams param;
	param.AddIgnoredActor(OwnerPC.Get());
	TArray<FOverlapResult> overlapResults;
	
	
	if(GetWorld()->OverlapMultiByChannel(overlapResults, OwnerPC->GetActorLocation(), FQuat::Identity, CCHANNEL_CharacterBase,
		FCollisionShape::MakeSphere(SphereAttackInfo.SphereRadius), param))
	{
		for(const FOverlapResult& result : overlapResults)
		{
			ACharacterBase* target = Cast<ACharacterBase>(result.GetActor());
			if(target == nullptr)
				continue;

			if(target->GetCharacterType() == TargetType)
			{
				FVector dirToActor = (target->GetActorLocation() - OwnerPC->GetActorLocation()).GetSafeNormal2D();
				float degree = FMath::RadiansToDegrees(acosf(FVector::DotProduct(OwnerPC->GetActorForwardVector(), dirToActor)));
				if(degree <= ArcAttackInfo.ArcAngle / 2)
				{
					FDamageEvent event;
					target->TakeDamage(10.f, event, OwnerPC->GetController(), OwnerPC.Get());
				}
			}
		}
	}
}

FVector USkillStampComponent::RotateVectorFromYaw(FVector InVector, float Degree)
{
	FVector result;
	float Radian = FMath::DegreesToRadians(Degree);

	float CosTheta = FMath::Cos(Radian);
	float SinTheta = FMath::Sin(Radian);

	result.X = InVector.X * CosTheta - InVector.Y * SinTheta;
	result.Y = InVector.X * SinTheta + InVector.Y * CosTheta;
	result.Z = 0;
	
	return result.GetSafeNormal();
}
