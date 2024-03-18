// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * UI Base클래스입니다.
 */
UCLASS()
class PROJECTJQ_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	TWeakObjectPtr<AActor>	OwnerActor;
	int32					UserWidgetId;
public:
	UWidget* operator[](const TCHAR* InWidgetName);

	void SetUserWidgetId(int32 InUserWidgetId);
	void SetOwnerActor(AActor* InActor){OwnerActor = InActor;}
	
	virtual void OnCreated() {}
};
