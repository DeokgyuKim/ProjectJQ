// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "../Data/ItemDataTable.h"
#include "../Component/StatControlComponent.h"

#include <Engine/Texture.h>
#include <Engine/StaticMesh.h>
#include <Components/StaticMeshComponent.h>

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
	ItemDescription = itemTable->ItemDescription;
	ItemStat = itemTable->ItemStat;
	UIImage = NewObject<UTexture>(itemTable->UIImage);
	if(UIImage == nullptr)
		return;

	ItemMesh = NewObject<UStaticMesh>(itemTable->ItemMesh);
	if(ItemMesh == nullptr)
		return;

	StaticMeshComp->SetStaticMesh(ItemMesh);

	//아이템 디테일 위젯 생성
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
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::OnPossess(AActor* InOwner)
{
	if(InOwner == nullptr)
		return;

	Owner = InOwner;

	UActorComponent* comp = Owner->GetComponentByClass(UStatControlComponent::StaticClass());
	if(comp == nullptr)
		return;

	UStatControlComponent* statComp = Cast<UStatControlComponent>(comp);

	for(const TPair<EStatControlType, double>& statValue : ItemStat)
		statComp->SetStat(statValue.Key, statComp->GetStat(statValue.Key) + statValue.Value);
}

void AItemActor::OnUnPossess(AActor* InOwner)
{
	if(Owner != InOwner)
		return;
	
	UActorComponent* comp = Owner->GetComponentByClass(UStatControlComponent::StaticClass());
	if(comp == nullptr)
		return;

	UStatControlComponent* statComp = Cast<UStatControlComponent>(comp);

	for(const TPair<EStatControlType, double>& statValue : ItemStat)
		statComp->SetStat(statValue.Key, statComp->GetStat(statValue.Key) - statValue.Value);
	
	Owner = nullptr;
}

