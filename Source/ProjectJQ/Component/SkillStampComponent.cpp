// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStampComponent.h"

#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "Engine/DamageEvents.h"
#include "ProjectJQ/Character/CharacterPC.h"

// Sets default values for this component's properties
USkillStampComponent::USkillStampComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USkillStampComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerPC = Cast<ACharacterPC>(GetOwner());
}


// Called every frame
void USkillStampComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillStampComponent::ActivateSkill()
{
	_ActivateSkill();
}

void USkillStampComponent::_ActivateSkill()
{
	//실제 작동 함수
	FCollisionQueryParams param;
	param.AddIgnoredActor(OwnerPC.Get());
	TArray<FOverlapResult> overlapResults;

	bool hitDetected = false;
	if(GetWorld()->OverlapMultiByChannel(overlapResults, OwnerPC->GetActorLocation() + OwnerPC->GetActorRotation().Quaternion() * ColliderBoxExtend / 2, OwnerPC->GetActorRotation().Quaternion(), CCHANNEL_CharacterBase,
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

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = hitDetected ? FColor::Green : FColor::Red;

	DrawDebugBox(GetWorld(), OwnerPC->GetActorLocation() + OwnerPC->GetActorRotation().Quaternion() * ColliderBoxExtend / 2,
		ColliderBoxExtend / 2, OwnerPC->GetActorRotation().Quaternion(), DrawColor, false, 5.0f);
#endif
}

void USkillStampComponent::SkillTriggered()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillTriggerd"), *GetName());

	for(const FSkillAnimMontageInfo& info : SkillAnimInfos)
		if(OwnerPC.IsValid() && info.PlayTiming == ETriggerEvent::Triggered)
			OwnerPC->PlayCharacterAnimMontage(1.f, info.SectionName);
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

