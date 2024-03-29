// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"

void UInventory::OnCreated()
{
	Super::OnCreated();

	Image_Items.Reserve(InventoryCount);
	for(int i = 0; i < InventoryCount; ++i)
	{
		FString buttonName = FString::Printf(TEXT("Image_Item_%d"), i);
		UWidget* widget = (*this)[*buttonName];
		
		if(widget == nullptr)
		{
			LOG_SCREEN(FColor::Red,TEXT("%s에 해당하는 UIWidget이 없습니다."), *buttonName)
			return;
		}

		UImage* image = Cast<UImage>(widget);
		if(image == nullptr)
		{
			LOG_SCREEN(FColor::Red,TEXT("%s에 해당하는 UIWidget이 UButton이 아닙니다."), *buttonName)
			return;
		}

		Image_Items.Add(image);
	}
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}
