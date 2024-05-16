// Fill out your copyright notice in the Description page of Project Settings.


#include "CpButtonList.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void UCpButtonList::OnCreated()
{
	Super::OnCreated();

	for(int i = 0; i < MaxButtonCount; ++i)
	{
		FString str = FString::Printf(TEXT("Button_%d"), i);
		if(UButton* button = Cast<UButton>((*this)[*str]))
			Buttons.Add(button);
		
		str = FString::Printf(TEXT("Text_%d"), i);
		if(UTextBlock* text = Cast<UTextBlock>((*this)[*str]))
			Texts.Add(text);
	}

	Buttons[0]->OnClicked.AddDynamic(this, &UCpButtonList::OnClickedButton_0);
	Buttons[1]->OnClicked.AddDynamic(this, &UCpButtonList::OnClickedButton_1);
	Buttons[2]->OnClicked.AddDynamic(this, &UCpButtonList::OnClickedButton_2);
	Buttons[3]->OnClicked.AddDynamic(this, &UCpButtonList::OnClickedButton_3);
	Buttons[4]->OnClicked.AddDynamic(this, &UCpButtonList::OnClickedButton_4);
	Buttons[5]->OnClicked.AddDynamic(this, &UCpButtonList::OnClickedButton_5);
	Buttons[6]->OnClicked.AddDynamic(this, &UCpButtonList::OnClickedButton_6);
}

void UCpButtonList::SetButtonTexts(TArray<FString> InButtonTexts)
{
	if(InButtonTexts.Num() < 0 || InButtonTexts.Num() > MaxButtonCount)
		return;

	for(int i = 0; i < MaxButtonCount; ++i)
	{
		if(i < InButtonTexts.Num())
		{
			Buttons[i]->SetVisibility(ESlateVisibility::Visible);
			Texts[i]->SetVisibility(ESlateVisibility::Visible);
			Texts[i]->SetText(FText::FromString(InButtonTexts[i]));
		}
		else
		{
			Buttons[i]->SetVisibility(ESlateVisibility::Collapsed);
			Texts[i]->SetVisibility(ESlateVisibility::Collapsed);
			Texts[i]->SetText(FText::FromString(TEXT("")));
		}
	}
}

void UCpButtonList::BindUFunctionDelegateCpButtonListOnClicked(UUserWidgetBase* InWidget, FName InFunctionName)
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Clear();

	if(!InWidget)
		return;
	
	DelegateCpButtonListOnClicked.AddUFunction(InWidget, InFunctionName);
}

void UCpButtonList::OnClickedButton_0()
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Broadcast(0);
}

void UCpButtonList::OnClickedButton_1()
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Broadcast(1);
}

void UCpButtonList::OnClickedButton_2()
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Broadcast(2);
}

void UCpButtonList::OnClickedButton_3()
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Broadcast(3);
}

void UCpButtonList::OnClickedButton_4()
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Broadcast(4);
}

void UCpButtonList::OnClickedButton_5()
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Broadcast(5);
}

void UCpButtonList::OnClickedButton_6()
{
	if(DelegateCpButtonListOnClicked.IsBound())
		DelegateCpButtonListOnClicked.Broadcast(6);
}
