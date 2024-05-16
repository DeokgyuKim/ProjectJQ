// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "JQSlotPure.generated.h"

class UItemSmallIcon;
class UImage;
class UBorder;
class UCanvasPanel;
class USlotDragDropOper;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonDownDelegate);

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UJQSlotPure : public UUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	////UI//////////////////////////////////////////////////////////////////////
	//캔버스 패널 UI
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCanvasPanel* CanvasPanel = nullptr;

	//백그라운드 보더 UI
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UBorder* Background = nullptr;

	//아이템 스몰 아이콘
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UItemSmallIcon* ItemSmallIcon = nullptr;
	
	////Setting//////////////////////////////////////////////////////////////////////
	//기본 텍스쳐
	UPROPERTY(EditAnywhere)
	UTexture2D* BasicTexture = nullptr;

	//호버링 텍스쳐
	UPROPERTY(EditAnywhere)
	UTexture2D* HoveringTexture = nullptr;

	//현재 슬롯 인덱스
	UPROPERTY(EditAnywhere)
	int32 SlotIndex = 0;
	
	////Info//////////////////////////////////////////////////////////////////////
	//아이템 Id
	UPROPERTY()
	int32 ItemId = -9999;

protected:
	virtual bool IsCanSwap(USlotDragDropOper* InSlotOper){return true;}
	
public:
	virtual void OnCreated() override;

public:
	void SetItem(int32 InItemID = -9999);
	int32 GetSlotIndex(){return SlotIndex;}

public:
	//마우스가 위젯 내부로 들어오면 호출됩니다. 호버링 텍스쳐를 적용합니다.
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//마우스가 외젯 외부로 떠나면 호출됩니다. 기본 텍스쳐로 다시 변경합니다.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	//마우스 버튼이 눌리면 호출됩니다. 왼쪽 클릭의 경우, 드래그 앤 드롭을 시작합니다.
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//마우스 버튼이 떼지면 호출됩니다.
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//드래그 앤 드롭 기능이 시작할 때 호출됩니다. 드래그 앤 드롭에 필요한 정보를 OutOperation에 담아 정보를 제공합니다.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	//드롭이 되었을 때 드롭 위치의 슬롯에서 호출됩니다. NativeOnDragDetected에서 넘겨준 OutOperation이 InOperation으로 들어옵니다. 아이템 정보를 뒤바꿔주고, 인벤토리를 리프레시합니다.
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
