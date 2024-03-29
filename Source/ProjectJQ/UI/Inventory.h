// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "Inventory.generated.h"

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

	TArray<TWeakObjectPtr<UImage>> Image_Items;

	static constexpr int32 InventoryCount = 50;

public:
	virtual void OnCreated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
