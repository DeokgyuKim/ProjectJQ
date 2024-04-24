// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "ProjectJQ/Character/CharacterBase.h"

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

	APlayerController* playerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if(playerController)
	{
		FVector2d mousePos = UWidgetLayoutLibrary::GetMousePositionOnPlatform();

		//커서가 스크롤 박스 밖으로 넘어갈 경우 검사 종료
		FVector2d start = ItemScroll->GetCachedGeometry().GetAbsolutePosition();
		FVector2d end = start + ItemScroll->GetCachedGeometry().GetAbsoluteSize();
		if(mousePos.X < start.X || mousePos.X > end.X || mousePos.Y < start.Y || mousePos.Y > end.Y)
			return;

		//커서가 호버링 위치 안에 있는 지 확인
		for(int i = 0; i < Image_Items.Num(); ++i)
		{
			FVector2d posMin = Image_Items[i]->GetCachedGeometry().GetAbsolutePosition();
			FVector2d posMax = posMin + Image_Items[i]->GetCachedGeometry().GetAbsoluteSize();

			//마우스가 호버링 되었을 때
			if(mousePos.X <= posMax.X && mousePos.X >= posMin.X && mousePos.Y <= posMax.Y && mousePos.Y >= posMin.Y)
			{
				//LOG_SCREEN(FColor::White, TEXT("Hovering to Image_%d"), i)
			}
		}
	}
}

void UInventory::SetOwner(TWeakObjectPtr<ACharacterBase> InCharacter)
{
	if(InCharacter.IsValid())
		OwnerCharacter = InCharacter;
}

FReply UInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FKey key = InMouseEvent.GetEffectingButton();
	LOG_SCREEN(FColor::White, TEXT("%s"), *key.ToString())
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
