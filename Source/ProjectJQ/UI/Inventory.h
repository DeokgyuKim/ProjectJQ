// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "Inventory.generated.h"

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
class PROJECTJQ_API UInventory : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<ACharacterBase> OwnerCharacter;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* Image_Helmet;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* Image_Armor;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* Image_RWeapon;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* Image_LWeapon;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* Image_Shoes;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* Image_Necklace;

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

	TArray<TWeakObjectPtr<UJQSlotPure>> ItemSlot;

public:
	virtual void OnCreated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetOwner(TWeakObjectPtr<ACharacterBase> InCharacter);

	void RefreshInventory(const TArray<TWeakObjectPtr<AItemActor>>& InItems);

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
