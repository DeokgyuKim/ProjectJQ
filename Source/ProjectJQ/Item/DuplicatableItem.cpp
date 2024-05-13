// Fill out your copyright notice in the Description page of Project Settings.


#include "DuplicatableItem.h"
#include "../Data/ItemDataTable.h"

// Sets default values
ADuplicatableItem::ADuplicatableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADuplicatableItem::BeginPlay()
{
	Super::BeginPlay();
	
	// 데이터 테이블 로드
	UDataTable* table = LOAD_DATA_TABLE(Item);
	if(table == nullptr)
		return;
	
	FItemDataTable* itemTable = table->FindRow<FItemDataTable>(*ItemName, ItemName);
	if(itemTable == nullptr)
		return;
}

// Called every frame
void ADuplicatableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
