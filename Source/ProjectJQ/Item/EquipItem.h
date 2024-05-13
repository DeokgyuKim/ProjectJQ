// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "EquipItem.generated.h"

UCLASS()
class PROJECTJQ_API AEquipItem : public AItemActor
{
	GENERATED_BODY()
protected:
	//아이템 스탯
	TMap<EStatControlType, double> ItemStat;

	//젬스톤
	EGemStoneType Gemstones[3] = {EGemStoneType::None};

	//장비 아이템의 타입
	EEquipItemType EquipItemType = EEquipItemType::Weapon;
public:	
	// Sets default values for this actor's properties
	AEquipItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	EEquipItemType GetEquipItemType(){return EquipItemType;}

public:
	// 아이템을 착용할 때 호출합니다.
	virtual void OnPossess(AActor* InOwner);

	// 아이템을 해제할 때 호출합니다.
	virtual void OnUnPossess(AActor* InOwner);
};
