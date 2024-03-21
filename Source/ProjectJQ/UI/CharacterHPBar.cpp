// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHPBar.h"
#include "../Character/CharacterBase.h"
#include "../Component/StatControlComponent.h"

#include <Components/ProgressBar.h>

void UCharacterHPBar::OnCreated()
{
	Super::OnCreated();

	if(!OwnerActor.IsValid())
		return;
	
	ACharacterBase* base = Cast<ACharacterBase>(OwnerActor);
	if(!base)
		return;

	UActorComponent* actorComp = base->GetComponentByClass(UStatControlComponent::StaticClass());
	if(!actorComp)
		return;

	UStatControlComponent* statComp = Cast<UStatControlComponent>(actorComp);

	MaxValue = statComp->GetStat(EStatControlType::HpMax);
	CurrentValue = statComp->GetStat(EStatControlType::Hp);
	GoalValue = CurrentValue;
	DiffValue = 0.0;

	HPBar->SetPercent(1.0);
	
	statComp->GetSetStatDelegate().AddUObject(this, &UCharacterHPBar::CallbackChangeHp);
}

void UCharacterHPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentValue += DiffValue * InDeltaTime / AnimSpeed;
	if(DiffValue < 0)
	{
		if(CurrentValue < GoalValue)
		{
			CurrentValue = GoalValue;
			DiffValue = 0.0;
		}
	}
	else if(DiffValue > 0)
	{
		if(CurrentValue > GoalValue)
		{
			CurrentValue = GoalValue;
			DiffValue = 0.0;
		}
	}
	
	HPBar->SetPercent(CurrentValue / MaxValue);
}

void UCharacterHPBar::CallbackChangeHp(TWeakObjectPtr<AActor> InActor, EStatControlType InStatControlType,
	double InValue)
{
	if(InStatControlType != EStatControlType::Hp)
		return;

	GoalValue = InValue;
	DiffValue = GoalValue - CurrentValue;
}
