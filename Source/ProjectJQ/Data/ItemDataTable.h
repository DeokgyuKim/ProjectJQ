// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataTable.h>
#include "ItemDataTable.generated.h"

/**
 * 아이템 데이터 테이블입니다.
 * 행 이름인 아이템 네임과 아이템 정보를 가지고 있습니다.
 */
USTRUCT()
struct PROJECTJQ_API FItemDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	//아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription = TEXT("");
	
	//장비 아이템의 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipItemType EquipItemType = EEquipItemType::Weapon;

	//아이템 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EStatControlType, double> ItemStat;

	//아이템 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* UIImage = nullptr;
	
	//월드에서 보여줄 스태틱 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PresetGemstone = false;

	//젬스톤 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="PresetGemstone"))
	EGemStoneType Gemstone1 = EGemStoneType::None;
	
	//젬스톤 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="PresetGemstone"))
	EGemStoneType Gemstone2 = EGemStoneType::None;
	
	//젬스톤 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="PresetGemstone"))
	EGemStoneType Gemstone3 = EGemStoneType::None;
};
