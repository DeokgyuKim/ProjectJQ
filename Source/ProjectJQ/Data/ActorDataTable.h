// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataTable.h>
#include "ActorDataTable.generated.h"

/**
 * 액터 데이터 테이블입니다.
 * 액터 BP_Path와 StatData를 가지고 있습니다.
 */
USTRUCT()
struct PROJECTJQ_API FActorDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	//BP 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BluePrintPath = TEXT("");

	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Hp = 100.0;

	//공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Attack = 10.0;

	//탐지 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double DetectRange = 600.0;
	
	//이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double MoveSpeed = 600.0;
};
