// Fill out your copyright notice in the Description page of Project Settings.


#include "JQWidgetComponent.h"

#include "ProjectJQ/UI/UserWidgetBase.h"

void UJQWidgetComponent::InitWidget()
{
	Super::InitWidget();
	UUserWidgetBase* widget = Cast<UUserWidgetBase>(GetWidget());
	if(widget)
	{
		widget->SetOwnerActor(GetOwner());
		widget->OnCreated();
	}
}
