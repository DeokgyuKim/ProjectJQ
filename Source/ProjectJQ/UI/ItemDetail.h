// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "ItemDetail.generated.h"

class UVerticalBox;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTJQ_API UItemDetail : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UTextBlock* ItemName;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UVerticalBox* ItemStat;

	UPROPERTY()
	TArray<TObjectPtr<UTextBlock>> StatTextBlocks;

public:
	virtual void OnCreated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void AddStatWidget(EStatControlType InStatControlType, double InValue);
};
