// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "CpButtonList.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateCpButtonListOnClicked, int32);
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UCpButtonList : public UUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	//아이템 이미지
	UPROPERTY(BlueprintReadWrite)
	TArray<UButton*> Buttons;

	//아이템 갯수
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> Texts;

	FDelegateCpButtonListOnClicked DelegateCpButtonListOnClicked;

	static constexpr int32 MaxButtonCount = 7;
public:
	virtual void OnCreated() override;

	void SetButtonTexts(TArray<FString> InButtonTexts);

	void ClearDelegateCpButtonListOnClicked(){DelegateCpButtonListOnClicked.Clear();}
	void BindUFunctionDelegateCpButtonListOnClicked(UUserWidgetBase* InWidget, FName InFunctionName);

	UFUNCTION()
	void OnClickedButton_0();
	UFUNCTION()
	void OnClickedButton_1();
	UFUNCTION()
	void OnClickedButton_2();
	UFUNCTION()
	void OnClickedButton_3();
	UFUNCTION()
	void OnClickedButton_4();
	UFUNCTION()
	void OnClickedButton_5();
	UFUNCTION()
	void OnClickedButton_6();
};
