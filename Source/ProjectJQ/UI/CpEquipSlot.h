// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CpSlotPure.h"
#include "CpEquipSlot.generated.h"

class UImage;
class UBorder;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UCpEquipSlot : public UCpSlotPure
{
	GENERATED_BODY()
	
protected:
	//장비 아이템의 타입
	UPROPERTY(EditAnywhere)
	EEquipItemType EquipItemType;

protected:
	virtual bool IsCanSwap(USlotDragDropOper* InSlotOper) override;
	
public:
	virtual void OnCreated() override;

};
