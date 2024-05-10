// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/ObjectManagementTargetInterface.h"
#include "ItemActor.generated.h"

class UItemDetail;
class UUserWidgetBase;

UCLASS()
class PROJECTJQ_API AItemActor : public AActor, public IObjectManagementTargetInterface
{
	GENERATED_BODY()

	//아이템 소유 액터
	TWeakObjectPtr<AActor> ItemOwner;

	//아이템 이름
	FString ItemName;
	
	//아이템 설명
	FString ItemDescription;

	//아이템 스탯
	TMap<EStatControlType, double> ItemStat;

	//젬스톤
	EGemStoneType Gemstones[3];

	//아이템 이미지
	TObjectPtr<UTexture2D> UIImage;
	
	//인벤토리 등에서 사용할 아이템 정보 디테일 위젯
	TObjectPtr<UItemDetail> ItemInfoDetailWidget;

	//월드에서 보여줄 스태틱 메쉬
	TObjectPtr<UStaticMesh> ItemMesh;

	//장비 아이템의 타입
	EEquipItemType EquipItemType;

	//아이템이 속해있는 타입
	EItemLocateType ItemLocateType;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UTexture2D* GetItemImage(){return UIImage;}

	EEquipItemType GetEquipItemType(){return EquipItemType;}

public:
	// 아이템을 착용할 때 호출합니다.
	virtual void OnPossess(AActor* InOwner);

	// 아이템을 해제할 때 호출합니다.
	virtual void OnUnPossess(AActor* InOwner);

	// 아이템 이름을 설정합니다.
	virtual void SetItemName(FString InItemName) {ItemName = InItemName;}

	void SetItemOwner(TWeakObjectPtr<AActor> InItemOwner) {ItemOwner = InItemOwner;}

	/*
	아이템이 속해있는 위치를 변경합니다.
	InItemLocateType이 World이면 액터를 보이게하고, InLocation의 위치로 옮깁니다.
	InItemLocateType이 Inventory라면 액터를 숨기고, -9999, -9999, -9999로 옮깁니다.
	*/
	void SetItemLocateType(EItemLocateType InItemLocateType, FVector InLocation = FVector::ZeroVector);
};
