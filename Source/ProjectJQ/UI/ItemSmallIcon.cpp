// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSmallIcon.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UItemSmallIcon::OnCreated()
{
	Super::OnCreated();
}

void UItemSmallIcon::SetImage(UTexture2D* InTexture)
{
	ItemImage->SetBrushFromTexture(InTexture);
}

void UItemSmallIcon::SetItemCount(int32 InCount)
{
	if(InCount == 0)
	{
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ItemCount->SetVisibility(ESlateVisibility::Visible);
		ItemCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), InCount)));
	}
}
