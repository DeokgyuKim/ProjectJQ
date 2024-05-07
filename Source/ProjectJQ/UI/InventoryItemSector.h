// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "InventoryItemSector.generated.h"

class AItemActor;
class UJQSlotPure;
class UScrollBox;
class ACharacterBase;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UInventoryItemSector : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<ACharacterBase> OwnerCharacter;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Sort_KindOrder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Sort_ValueOrder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Sort_GetOrder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Exit;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UScrollBox* ItemScroll;

	//아이템 슬롯 UI
	TArray<TWeakObjectPtr<UJQSlotPure>> ItemSlot;

public:
	virtual void OnCreated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetOwner(TWeakObjectPtr<ACharacterBase> InCharacter);

	//인벤토리 UI를 넘겨받은 InItems로 리프레시 합니다.
	void RefreshInventory(const TArray<TWeakObjectPtr<AItemActor>>& InItems);
};
