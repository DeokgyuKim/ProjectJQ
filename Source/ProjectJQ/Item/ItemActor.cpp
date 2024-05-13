// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
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
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
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
	AcquireTime = FDateTime::Now();
	ItemDescription = itemTable->ItemDescription;
	ItemType = itemTable->ItemType;
	EquipItemType = itemTable->EquipItemType;
	ItemValue = itemTable->ItemValue;
	ItemStat = itemTable->ItemStat;
	UIImage = itemTable->UIImage;

	//스태틱 메시
	ItemMesh = itemTable->ItemMesh;
	StaticMeshComp->SetStaticMesh(ItemMesh);

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

	//아이템 디테일 위젯 생성

	/*
	UUIManagementGSS* gss = GetGameInstance()->GetSubsystem<UUIManagementGSS>();
	if(gss == nullptr)
		return;
	int32 id = gss->CreateWidgetBase(TEXT("WBP_ItemDetail"), TEXT("ItemDetail_") + GetName(), this);

	if(id == UUIManagementGSS::INVALID_UIID)
		return;

	UUserWidgetBase* widget = gss->FindWidgetBase(id);
	if(widget == nullptr)
		return;

	ItemInfoDetailWidget = Cast<UItemDetail>(widget);
	if(ItemInfoDetailWidget == nullptr)
		return;
	

	//아이템 디테일 위젯에 정보 추가
	for(const TPair<EStatControlType, double>& info : ItemStat)
		ItemInfoDetailWidget->AddStatWidget(info.Key, info.Value);
	*/
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ItemLocateType == EItemLocateType::World)
	{
		if(UObjectManagementGSS* gss = GetGameInstance()->GetSubsystem<UObjectManagementGSS>())
		{
			TArray<ACharacterPC*> pcs = gss->GetPlayers();
			if(pcs.IsEmpty())
				return;

			//플레이어중 아이템과 가까운 순으로 정렬
			FVector itemLoc = GetActorLocation();
			pcs.Sort([itemLoc](ACharacterPC& lhs, ACharacterPC& rhs)
			{
				return FVector::Distance(itemLoc, lhs.GetActorLocation()) < FVector::Distance(itemLoc, rhs.GetActorLocation());
			});

			//제일 가까운 플레이어의 거리가 100이하라면 아이템 획득
			if(FVector::Distance(pcs[0]->GetActorLocation(), itemLoc) < 100)
			{
				if(UInventoryComponent* comp = Cast<UInventoryComponent>(pcs[0]->GetComponentByClass(UInventoryComponent::StaticClass())))
				{
					comp->AcquireItem(this);
					return;
				}
			}
		}
	}
}

void AItemActor::OnPossess(AActor* InOwner)
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

void AItemActor::OnUnPossess(AActor* InOwner)
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

void AItemActor::SetItemLocateType(EItemLocateType InItemLocateType, FVector InLocation)
{
	if(ItemLocateType == InItemLocateType)
		return;

	ItemLocateType = InItemLocateType;
	switch (ItemLocateType)
	{
	case EItemLocateType::Inventroy:
		SetActorHiddenInGame(true);
		SetActorLocation(FVector(-9999.0, -9999.0, -9999.0));
		break;
	case EItemLocateType::World:
		SetActorHiddenInGame(false);
		SetActorLocation(InLocation);
		break;
	default:
		break;
	}
}
