// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "JQSlotPure.generated.h"

class UImage;
class UBorder;
class UCanvasPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonDownDelegate);

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UJQSlotPure : public UUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCanvasPanel* CanvasPanel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UBorder* Background = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* ItemImage = nullptr;

	UTexture2D* ItemTexture = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* BasicTexture = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* HoveringTexture = nullptr;

	UPROPERTY(EditAnywhere)
	int32 SlotIndex = 0;
	
public:
	virtual void OnCreated() override;

public:
	void SetItem(UTexture2D* InItemImage);
	UTexture2D* GetItemTexture2D(){return ItemTexture;}
	int32 GetSlotIndex(){return SlotIndex;}

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
