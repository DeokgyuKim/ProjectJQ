// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "Inventory.generated.h"

class UCpButtonList;
class UInventoryItemSector;
class AItemActor;
class UCpEquipSlot;
class UScrollBox;
class ACharacterBase;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UInventory : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<ACharacterBase> OwnerCharacter = nullptr;

	UPROPERTY()
	TMap<EEquipItemUIType, UCpEquipSlot*> EquipSlot;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCpEquipSlot* Slot_Helmet = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCpEquipSlot* Slot_Armor = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCpEquipSlot* Slot_RWeapon = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCpEquipSlot* Slot_LWeapon = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCpEquipSlot* Slot_Shoes = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCpEquipSlot* Slot_Necklace = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Exit = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UInventoryItemSector* ItemSector = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCpButtonList* SlotButtonList = nullptr;

	TArray<FString> SlotButtonListTexts;

	TWeakObjectPtr<AItemActor> ClickedItem = nullptr;

public:
	virtual void OnCreated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetOwner(TWeakObjectPtr<ACharacterBase> InCharacter);

	//인벤토리 UI를 넘겨받은 InItems로 리프레시 합니다.
	void RefreshInventory(const TMap<EEquipItemUIType, TWeakObjectPtr<AItemActor>> InEquipItems, const TArray<TWeakObjectPtr<AItemActor>>& InItems);

	UFUNCTION()
	void RightButtonClickedSlot(UCpSlotPure* InSlot, int32 InItemID);

	UFUNCTION()
	void OnClickedSlotButtonList(int32 InButtonIndex);
	
};
