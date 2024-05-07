// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "Inventory.generated.h"

class UInventoryItemSector;
class AItemActor;
class UJQEquipSlot;
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
	TWeakObjectPtr<ACharacterBase> OwnerCharacter;

	UPROPERTY()
	TMap<EEquipItemUIType, UJQEquipSlot*> EquipSlot;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJQEquipSlot* Slot_Helmet;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJQEquipSlot* Slot_Armor;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJQEquipSlot* Slot_RWeapon;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJQEquipSlot* Slot_LWeapon;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJQEquipSlot* Slot_Shoes;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJQEquipSlot* Slot_Necklace;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Sort_KindOrder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Sort_ValueOrder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Sort_GetOrder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Button_Exit;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UInventoryItemSector* ItemSector;

public:
	virtual void OnCreated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetOwner(TWeakObjectPtr<ACharacterBase> InCharacter);

	//인벤토리 UI를 넘겨받은 InItems로 리프레시 합니다.
	void RefreshInventory(const TMap<EEquipItemUIType, TWeakObjectPtr<AItemActor>> InEquipItems, const TArray<TWeakObjectPtr<AItemActor>>& InItems);
};
