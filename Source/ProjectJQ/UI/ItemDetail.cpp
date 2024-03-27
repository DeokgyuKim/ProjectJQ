// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDetail.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UItemDetail::OnCreated()
{
	Super::OnCreated();

}

void UItemDetail::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UItemDetail::AddStatWidget(EStatControlType InStatControlType, double InValue)
{
	UWidget* widget = CreateWidget(this, UTextBlock::StaticClass());
	UTextBlock* textBlock = Cast<UTextBlock>(widget);

	if(textBlock == nullptr)
		return;

	FString itemInfo = UtilPath::EnumToString<EStatControlType>(InStatControlType);
	itemInfo += FString::Printf(TEXT(": %lf"), InValue);

	FSlateFontInfo info;
	info.Size = 18;
	textBlock->SetFont(info);
	textBlock->SetText(FText::FromString(itemInfo));

	ItemStat->AddChild(textBlock);

	StatTextBlocks.Add(textBlock);
}
