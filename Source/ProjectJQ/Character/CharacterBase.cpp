// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"
#include "../Component/StatControlComponent.h"
#include "../Component/DecoratorComponent.h"

#include <Animation/AnimInstance.h>
#include <Components/SkeletalMeshComponent.h>

#include "Animation/AnimMontage.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"

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

void ACharacterBase::CallBackDeadCharacter()
{
	if(DelegateDeadCharacter.IsBound())
		DelegateDeadCharacter.Broadcast();

	/*
	if(CharacterType == ECharacterType::Monster)
	{
		UObjectManagementGSS* gss = GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
		if(gss)
			gss->DestroyActor(this);
	}
	*/
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float CurrentHp = StatControlComponent->GetStat(EStatControlType::Hp);
	StatControlComponent->SetStat(EStatControlType::Hp, CurrentHp - DamageAmount);
	UE_LOG(LogTemp, Log, TEXT("%s takes %f Damage"), *GetName(), DamageAmount);
	return 0.0f;
}

float ACharacterBase::PlayCharacterAnimMontage(float InPlayRate, FName InSectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if(animInstance == nullptr || AnimMontage == nullptr)
		return INVALID_ANIMMONTAGE;

	if(!AnimMontage->IsValidSectionName(InSectionName))
		return INVALID_ANIMMONTAGE;
	
	if(animInstance->Montage_Play(AnimMontage, InPlayRate, EMontagePlayReturnType::Duration) <= 0.f)
		return INVALID_ANIMMONTAGE;
	
	animInstance->Montage_JumpToSection(InSectionName);
	
	int32 index = AnimMontage->GetSectionIndex(InSectionName);
	return AnimMontage->GetSectionLength(index) / InPlayRate;
}
