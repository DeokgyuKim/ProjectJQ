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
	FString ItemDescription;

	//아이템 스탯
	TMap<EStatControlType, double> ItemStat;

	//아이템 이미지
	TSubclassOf<UTexture> UIImage;
	
	//월드에서 보여줄 스태틱 메쉬
	TSubclassOf<UStaticMesh> ItemMesh;
};
