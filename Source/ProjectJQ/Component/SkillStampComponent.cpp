// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStampComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "../Actor/JQProjectile.h"
#include "../Character/CharacterPC.h"
#include "../SubSystem/ObjectManagementGSS.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"

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
	case EAttackRangeType::None:
		break;
	}
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
	return;
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
	
	AJQProjectile* ProjectileActor = gss->CreateActor<AJQProjectile>(ProjectileObject, param);
	
	if(ProjectileActor)
	{
		ProjectileActor->Initialize(OwnerPC->GetActorForwardVector(), OwnerPC->GetController());
	}
}

void USkillStampComponent::SkillStarted()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillStarted"), *GetName());

	if(DecalMaterial != nullptr)
	{
		DecalActor = GetWorld()->SpawnActor<ADecalActor>(GetWorldLocationAtMousePointer(), FRotator::ZeroRotator);
		DecalActor->SetDecalMaterial(DecalMaterial);
		UDecalComponent* decalComponent = DecalActor->GetDecal();
		
		//데칼의 크기를 조정합니다.
		switch (AttackRangeType)
		{
		case EAttackRangeType::Sphere:
			decalComponent->DecalSize = FVector(300, Length * 2, Length * 2);
			break;
		case EAttackRangeType::Projectile:
			decalComponent->DecalSize = FVector(Length, Length, 300);
			break;
		}
		return;
	}

	for(const FSkillAnimMontageInfo& info : SkillAnimInfos)
		if(OwnerPC.IsValid() && info.PlayTiming == ETriggerEvent::Started)
			OwnerPC->PlayCharacterAnimMontage(1.f, info.SectionName);
}

void USkillStampComponent::SkillTriggered()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillTriggerd"), *GetName());

	if(DecalActor->IsValidLowLevel())
	{
		switch (AttackRangeType)
		{
		case EAttackRangeType::Sphere:
			DecalActor->SetActorLocation(OwnerPC->GetActorLocation());
			break;
		case EAttackRangeType::Projectile:
			FVector location = (OwnerPC->GetActorLocation() + GetVector2DFromCharacterToMousePointer() * (OwnerPC->GetCapsuleComponent()->GetScaledCapsuleRadius() + Length));
			FRotator rotation = GetVector2DFromCharacterToMousePointer().Rotation();
			DecalActor->SetActorLocationAndRotation(location, FRotator(90, rotation.Yaw + 90, 0));
			break;
		}
		return;
	}

	FSkillAnimMontageInfo* animInfo = EventSkillsMap.Find(ETriggerEvent::Triggered);
	
	if(animInfo == nullptr) return;
	if(!OwnerPC.IsValid()) return;
	if(Count > animInfo->ComboCount) return;
	
	FString inputSectionName = animInfo->SectionName.ToString();
	FName NextSection = *FString::Printf(TEXT("%s%d"), *inputSectionName, Count);
	OwnerPC->PlayCharacterAnimMontage(1.f, NextSection);
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

	if(DecalActor->IsValidLowLevel())
	{
		DecalActor->Destroy();
	}

	for(const FSkillAnimMontageInfo& info : SkillAnimInfos)
		if(OwnerPC.IsValid() && info.PlayTiming == ETriggerEvent::Completed)
			OwnerPC->PlayCharacterAnimMontage(1.f, info.SectionName);
}

FVector USkillStampComponent::GetWorldLocationAtMousePointer()
{
	FHitResult hitResult;
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if(playerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult))
	{
		return hitResult.ImpactPoint;
	}
	if(DecalActor->IsValidLowLevel())
	{
		DecalActor->Destroy();
	}
	return FVector::ZeroVector;
}

FVector USkillStampComponent::GetVector2DFromCharacterToMousePointer()
{
	const FVector direction = GetWorldLocationAtMousePointer() - OwnerPC->GetActorLocation();
	return FVector(direction.X, direction.Y, 0).GetSafeNormal();
}

void USkillStampComponent::SetCharacterRotationToMousePointer(FVector InVectorToMouse) const
{
	const FVector direction2D = FVector(InVectorToMouse.X, InVectorToMouse.Y, 0);
	OwnerPC->SetActorRotation(direction2D.Rotation());
}


