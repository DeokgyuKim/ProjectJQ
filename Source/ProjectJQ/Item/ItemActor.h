// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

class UItemDetail;
class UUserWidgetBase;

UCLASS()
class PROJECTJQ_API AItemActor : public AActor
{
	GENERATED_BODY()

	//아이템 소유 액터
	TWeakObjectPtr<AActor> Owner;

	//아이템 이름
	FString ItemName;
	//아이템 설명
	FString ItemDescription;

	//아이템 스탯
	TMap<EStatControlType, double> ItemStat;

	//젬스톤
	EGemStoneType Gemstones[3];

	//아이템 이미지
	TObjectPtr<UTexture> UIImage;
	
	//인벤토리 등에서 사용할 아이템 정보 디테일 위젯
	TObjectPtr<UItemDetail> ItemInfoDetailWidget;

	//월드에서 보여줄 스태틱 메쉬
	TObjectPtr<UStaticMesh> ItemMesh;

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
	// 아이템 이름을 설정합니다.
	virtual void Initialize(FString InItemName){ItemName = InItemName;}

	// 아이템을 착용할 때 호출합니다.
	virtual void OnPossess(AActor* InOwner);

	// 아이템을 해제할 때 호출합니다.
	virtual void OnUnPossess(AActor* InOwner);
};
