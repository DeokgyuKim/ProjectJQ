// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JQSlotPure.h"
#include "JQEquipSlot.generated.h"

class UImage;
class UBorder;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UJQEquipSlot : public UJQSlotPure
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
