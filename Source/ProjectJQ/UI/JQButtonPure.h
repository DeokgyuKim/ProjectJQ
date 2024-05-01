// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "JQButtonPure.generated.h"

class UBorder;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PROJECTJQ_API UJQButtonPure : public UUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UCanvasPanel* CanvasPanel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UBorder* Background = nullptr;
	
public:
	virtual void OnCreated() override;
};
