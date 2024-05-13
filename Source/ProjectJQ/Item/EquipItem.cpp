// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItem.h"
#include "../Data/ItemDataTable.h"
#include "../Component/StatControlComponent.h"

#include <Engine/Texture2D.h>
#include <Engine/StaticMesh.h>
#include <Components/StaticMeshComponent.h>

#include "ProjectJQ/Character/CharacterPC.h"
#include "ProjectJQ/Component/InventoryComponent.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"
#include "ProjectJQ/SubSystem/UIManagementGSS.h"
#include "ProjectJQ/UI/ItemDetail.h"

// Sets default values
AEquipItem::AEquipItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEquipItem::BeginPlay()
{
	Super::BeginPlay();
	
	// 데이터 테이블 로드
	UDataTable* table = LOAD_DATA_TABLE(Item);
	if(table == nullptr)
		return;
	
	FItemDataTable* itemTable = table->FindRow<FItemDataTable>(*ItemName, ItemName);
	if(itemTable == nullptr)
		return;

	// 데이터 테이블 기반으로 아이템 정보 추가
	EquipItemType = itemTable->EquipItemType;
	ItemStat = itemTable->ItemStat;

	//젬스톤 세팅
	if(itemTable->PresetGemstone)
	{
		Gemstones[0] = itemTable->Gemstone1;
		Gemstones[1] = itemTable->Gemstone2;
		Gemstones[2] = itemTable->Gemstone3;
	}
	else
	{
		//프리셋이 되어있는 아이템이 아닌 경우 랜덤 젬스톤 삽입
		for(int i = 0; i < 3; ++i)
			Gemstones[i] = static_cast<EGemStoneType>(FMath::RandRange(static_cast<uint8>(EGemStoneType::None), static_cast<uint8>(EGemStoneType::Sapphire)));
	}
}

// Called every frame
void AEquipItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEquipItem::OnPossess(AActor* InOwner)
{
	if(InOwner == nullptr)
		return;

	//아이템 소유자와 착용자가 다릅니다.
	if(ItemOwner != InOwner)
		return;

	UActorComponent* comp = ItemOwner->GetComponentByClass(UStatControlComponent::StaticClass());
	if(comp == nullptr)
		return;

	UStatControlComponent* statComp = Cast<UStatControlComponent>(comp);

	for(const TPair<EStatControlType, double>& statValue : ItemStat)
		statComp->SetStat(statValue.Key, statComp->GetStat(statValue.Key) + statValue.Value);
}

void AEquipItem::OnUnPossess(AActor* InOwner)
{
	if(ItemOwner != InOwner)
		return;
	
	UActorComponent* comp = ItemOwner->GetComponentByClass(UStatControlComponent::StaticClass());
	if(comp == nullptr)
		return;

	UStatControlComponent* statComp = Cast<UStatControlComponent>(comp);

	for(const TPair<EStatControlType, double>& statValue : ItemStat)
		statComp->SetStat(statValue.Key, statComp->GetStat(statValue.Key) - statValue.Value);
}
