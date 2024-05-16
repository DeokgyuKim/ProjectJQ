// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "ItemSmallIcon.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UItemSmallIcon : public UUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	//아이템 이미지
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UImage* ItemImage = nullptr;

	//아이템 갯수
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UTextBlock* ItemCount = nullptr;
	
public:
	virtual void OnCreated() override;

	UImage* GetImage(){return ItemImage;}

	void SetImage(UTexture2D* InTexture);

	void SetItemCount(int32 InCount = 0);
};
