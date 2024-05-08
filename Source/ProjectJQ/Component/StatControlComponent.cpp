// Fill out your copyright notice in the Description page of Project Settings.


#include "StatControlComponent.h"
#include "../Data/ActorDataTable.h"
#include "../Character/CharacterBase.h"
#include "../Raw/Buff.h"

#include <GameFramework/CharacterMovementComponent.h>

// Sets default values for this component's properties
UStatControlComponent::UStatControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatControlComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if(!Owner.IsValid())
		return;
	
	//스탯 초기화
	Stat.Empty();
	for(EStatControlType statType : TEnumRange<EStatControlType>())
	{
		if(statType == EStatControlType::Count)
			continue;

		Stat.FindOrAdd(statType) = 0.0;
	}

	DelegateChangeStat.Clear();
	DelegateChangeStat.AddUObject(this, &UStatControlComponent::ChangeStatDelegateFunction);

	if(ActorName == TEXT(""))
	{
		UE_LOG(LogLoad, Error, TEXT("%s의 ActorName이 등록되지 않았습니다. 데이터 테이블에서 스탯을 불러올 수 없습니다."), *Owner->GetName())
		return;
	}

	UDataTable* table = LOAD_DATA_TABLE(Actor);
	FActorDataTable* data = table->FindRow<FActorDataTable>(*ActorName, ActorName);
	if(!data)
	{
		UE_LOG(LogLoad, Error, TEXT("ActorName: %s에 해당하는 데이터 열이 없습니다. 데이터 테이블에서 스탯을 불러올 수 없습니다."), *Owner->GetName())
		return;
	}

	SetStat(EStatControlType::Hp, data->Hp);
	SetStat(EStatControlType::HpMax, data->Hp);
	SetStat(EStatControlType::Attack, data->Attack);
	SetStat(EStatControlType::DetectRange, data->DetectRange);
	SetStat(EStatControlType::MoveSpeed, data->MoveSpeed);
}

void UStatControlComponent::ChangeStatDelegateFunction(TWeakObjectPtr<AActor> InActor, EStatControlType InStatType, double InValue)
{
	if(!InActor.IsValid())
		return;

	if(InStatType == EStatControlType::MoveSpeed)
	{
		ACharacterBase* charBase = Cast<ACharacterBase>(InActor);
		UActorComponent* comp = charBase->GetComponentByClass(UCharacterMovementComponent::StaticClass());
		if(comp == nullptr)
			return;

		UCharacterMovementComponent* movementComponent = Cast<UCharacterMovementComponent>(comp);
		movementComponent->MaxWalkSpeed = InValue;
		movementComponent->MaxWalkSpeedCrouched = InValue / 2;
	}
	else if(InStatType == EStatControlType::Hp)
	{
		if(InValue <= 0.0)
		{
			ACharacterBase* charBase = Cast<ACharacterBase>(InActor);
			charBase->CallBackDeadCharacter();
		}
	}
}

void UStatControlComponent::SetStat(EStatControlType InStatType, double InValue)
{
	double* stat = Stat.Find(InStatType);
	if(!stat || *stat == InValue)
		return;

	*stat = InValue;

	if(DelegateChangeStat.IsBound())
		DelegateChangeStat.Broadcast(Owner, InStatType, *stat);
}


// Called every frame
void UStatControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	for(TMap<FString, FBuff*>::TIterator iter = Buffs.CreateIterator(); iter; ++iter)
	{
		if(iter->Value->Tick(DeltaTime))
			iter.RemoveCurrent();
	}
}

const double& UStatControlComponent::GetStat(EStatControlType InStatType)
{
	const double* stat = Stat.Find(InStatType);
	if(!stat)
		return INVALID_STAT;

	return *stat;
}

void UStatControlComponent::AddBuff(const FString& InBuffName)
{
	FBuff** findTemp = Buffs.Find(InBuffName);
	
	FBuff* buff = nullptr;
	if(findTemp != nullptr)
	{
		buff = *findTemp;
		buff->Initialize();
	}
	else
	{
		buff = new FBuff(this, InBuffName);
		Buffs.Add(InBuffName, buff);
	}
}