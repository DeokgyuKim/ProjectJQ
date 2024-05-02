// Fill out your copyright notice in the Description page of Project Settings.


#include "PCSkillStampComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "../Actor/JQProjectile.h"
#include "../Character/CharacterPC.h"
#include "../SubSystem/ObjectManagementGSS.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values for this component's properties
UPCSkillStampComponent::UPCSkillStampComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPCSkillStampComponent::BeginPlay()
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
void UPCSkillStampComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPCSkillStampComponent::SkillStarted()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillStarted"), *GetName());

	if(DecalMaterial)
	{
		DecalActor = GetWorld()->SpawnActor<ADecalActor>(GetWorldLocationAtMousePointer(), FRotator::ZeroRotator);
		DecalActor->SetDecalMaterial(DecalMaterial);
		UDecalComponent* decalComponent = DecalActor->GetDecal();
		
		//데칼의 크기를 조정합니다.
		switch (AttackRangeType)
		{
		case EAttackRangeType::Sphere:
			decalComponent->DecalSize = FVector(300, SphereAttackInfo.SphereRadius * 2, SphereAttackInfo.SphereRadius * 2);
			break;
		case EAttackRangeType::Projectile:
			decalComponent->DecalSize = FVector(ProjectileAttackInfo.Length / 2, ProjectileAttackInfo.Length / 2, 300);
			break;
		case EAttackRangeType::Arc:
			UMaterialInstanceDynamic* dynMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, this);
			dynMaterial->SetScalarParameterValue(FName("Angle"), ArcAttackInfo.ArcAngle);
			DecalActor->SetDecalMaterial(dynMaterial);
			decalComponent->DecalSize = FVector(300, ArcAttackInfo.Radius * 2, ArcAttackInfo.Radius * 2);
			break;
		}
		return;
	}
	
	FSkillAnimMontageInfo* currentInfo = EventSkillsMap.Find(ETriggerEvent::Started);
	if(OwnerPC.IsValid() && currentInfo != nullptr && OwnerPC->GetCanAttack() )
	{
		SetCharacterRotationToMousePointer(GetVector2DFromCharacterToMousePointer());
		FName playSection = *FString::Printf(TEXT("%s%d"), *currentInfo->SectionName.ToString(), Count);
		OwnerPC->PlayCharacterAnimMontage(1.f, playSection);
		OwnerPC->SetCanAttack(false);
	}
}

void UPCSkillStampComponent::SkillTriggered()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillTriggerd"), *GetName());

	if(DecalMaterial != nullptr)
	{
		FVector location;
		FRotator rotation;
		switch (AttackRangeType)
		{
		case EAttackRangeType::Sphere:
			DecalActor->SetActorLocation(OwnerPC->GetActorLocation());
			break;
		case EAttackRangeType::Projectile:
			location = (OwnerPC->GetActorLocation() + GetVector2DFromCharacterToMousePointer() * (OwnerPC->GetCapsuleComponent()->GetScaledCapsuleRadius() + ProjectileAttackInfo.Length / 2));
			rotation = GetVector2DFromCharacterToMousePointer().Rotation();
			DecalActor->SetActorLocationAndRotation(location, FRotator(90, rotation.Yaw + 90, 0));
			break;
		case EAttackRangeType::Arc:
			location = OwnerPC->GetActorLocation();
			rotation = GetVector2DFromCharacterToMousePointer().Rotation();
			DecalActor->SetActorLocationAndRotation(location, FRotator(90, rotation.Yaw + 180, 0));
			break;
		}
		return;
	}

	FSkillAnimMontageInfo* currentInfo = EventSkillsMap.Find(ETriggerEvent::Triggered);
	if(OwnerPC.IsValid() && currentInfo != nullptr && OwnerPC->GetCanAttack())
	{
		SetCharacterRotationToMousePointer(GetVector2DFromCharacterToMousePointer());
		FName playSection = *FString::Printf(TEXT("%s%d"), *currentInfo->SectionName.ToString(), Count);
		OwnerPC->PlayCharacterAnimMontage(1.f, playSection);
		OwnerPC->SetCanAttack(false);
	}

	// FSkillAnimMontageInfo* animInfo = EventSkillsMap.Find(ETriggerEvent::Triggered);
	//
	// if(animInfo == nullptr) return;
	// if(!OwnerPC.IsValid()) return;
	// if(Count > animInfo->ComboCount) return;
	//
	// FString inputSectionName = animInfo->SectionName.ToString();
	// FName NextSection = *FString::Printf(TEXT("%s%d"), *inputSectionName, Count);
	// OwnerPC->PlayCharacterAnimMontage(1.f, NextSection);
	// OwnerPC->SetCanAttack(false);
}

void UPCSkillStampComponent::SkillOnGoing()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillOnGoing"), *GetName());

	FSkillAnimMontageInfo* currentInfo = EventSkillsMap.Find(ETriggerEvent::Ongoing);
	if(OwnerPC.IsValid() && currentInfo != nullptr && OwnerPC->GetCanAttack())
	{
		SetCharacterRotationToMousePointer(GetVector2DFromCharacterToMousePointer());
		FName playSection = *FString::Printf(TEXT("%s%d"), *currentInfo->SectionName.ToString(), Count);
		OwnerPC->PlayCharacterAnimMontage(1.f, playSection);
		OwnerPC->SetCanAttack(false);
	}
}

void UPCSkillStampComponent::SkillCanceled()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillCanceled"), *GetName());

	FSkillAnimMontageInfo* currentInfo = EventSkillsMap.Find(ETriggerEvent::Canceled);
	if(OwnerPC.IsValid() && currentInfo != nullptr && OwnerPC->GetCanAttack())
	{
		SetCharacterRotationToMousePointer(GetVector2DFromCharacterToMousePointer());
		FName playSection = *FString::Printf(TEXT("%s%d"), *currentInfo->SectionName.ToString(), Count);
		OwnerPC->PlayCharacterAnimMontage(1.f, playSection);
		OwnerPC->SetCanAttack(false);
	}
}

void UPCSkillStampComponent::SkillCompleted()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillCompleted"), *GetName());
	
	if(DecalMaterial != nullptr)
	{
		DecalActor->Destroy();
	}
	
	FSkillAnimMontageInfo* currentInfo = EventSkillsMap.Find(ETriggerEvent::Completed);
	if(OwnerPC.IsValid() && currentInfo != nullptr && OwnerPC->GetCanAttack())
	{
		SetCharacterRotationToMousePointer(GetVector2DFromCharacterToMousePointer());
		FName playSection = *FString::Printf(TEXT("%s%d"), *currentInfo->SectionName.ToString(), Count);
		OwnerPC->PlayCharacterAnimMontage(1.f, playSection);
		OwnerPC->SetCanAttack(false);
	}
}

FVector UPCSkillStampComponent::GetWorldLocationAtMousePointer()
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

FVector UPCSkillStampComponent::GetVector2DFromCharacterToMousePointer()
{
	const FVector direction = GetWorldLocationAtMousePointer() - OwnerPC->GetActorLocation();
	return FVector(direction.X, direction.Y, 0).GetSafeNormal();
}

void UPCSkillStampComponent::SetCharacterRotationToMousePointer(FVector InVectorToMouse) const
{
	const FVector direction2D = FVector(InVectorToMouse.X, InVectorToMouse.Y, 0);
	OwnerPC->SetActorRotation(direction2D.Rotation());
}