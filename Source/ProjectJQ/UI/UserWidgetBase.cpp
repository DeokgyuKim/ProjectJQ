// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetBase.h"

#include <Blueprint/WidgetTree.h>

UWidget* UUserWidgetBase::operator[](const TCHAR* InWidgetName)
{
	return WidgetTree->FindWidget(InWidgetName);
}

void UUserWidgetBase::SetUserWidgetId(int32 InUserWidgetId)
{
	UserWidgetId = InUserWidgetId;
}
