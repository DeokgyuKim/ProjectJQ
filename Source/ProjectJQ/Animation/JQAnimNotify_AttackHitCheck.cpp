// Fill out your copyright notice in the Description page of Project Settings.


#include "JQAnimNotify_AttackHitCheck.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProjectJQ/Character/CharacterPC.h"
#include "ProjectJQ/Component/AttackComponent.h"
#include "ProjectJQ/Interface/JQAnimationAttackInterface.h"

void UJQAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		ACharacterPC* AttackPawn = Cast<ACharacterPC>(MeshComp->GetOwner());
		if(AttackPawn)
		{
			UAttackComponent* AttackComponent = AttackPawn->GetComponentByClass<UAttackComponent>();
			if(AttackComponent)
			{
				AttackComponent->AttackHitCheck();
			}
		}
	}
}
