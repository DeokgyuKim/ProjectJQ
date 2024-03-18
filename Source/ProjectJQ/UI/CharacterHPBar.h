// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "CharacterHPBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTJQ_API UCharacterHPBar : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	double GoalValue;
	double CurrentValue;
	double DiffValue;
	double MaxValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_UISetting", meta=(ClampMin="0.1", ClampMax="1.0"))
	double AnimSpeed = 1.0;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UProgressBar* HPBar;

public:
	virtual void OnCreated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CallbackChangeHp(TWeakObjectPtr<AActor> InActor, EStatControlType InStatControlType, double InValue);
};
