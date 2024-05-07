// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemSector.h"

#include "JQSlotPure.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "ProjectJQ/Character/CharacterBase.h"
#include "ProjectJQ/Component/InventoryComponent.h"
#include "ProjectJQ/Item/ItemActor.h"

void UInventoryItemSector::OnCreated()
{
	Super::OnCreated();

	ItemSlot.Reserve(UInventoryComponent::TotalItemCount);
	for(int i = 0; i < UInventoryComponent::TotalItemCount; ++i)
	{
		FString slotName = FString::Printf(TEXT("ItemSlot_%d"), i);
		UWidget* widget = (*this)[*slotName];
		
		if(widget == nullptr)
		{
			LOG_SCREEN(FColor::Red,TEXT("%s에 해당하는 UIWidget이 없습니다."), *slotName)
			return;
		}

		UJQSlotPure* slot = Cast<UJQSlotPure>(widget);
		if(slot == nullptr)
		{
			LOG_SCREEN(FColor::Red,TEXT("%s에 해당하는 UIWidget이 UButton이 아닙니다."), *slotName)
			return;
		}

		ItemSlot.Add(slot);
		slot->OnCreated();
	}
}

void UInventoryItemSector::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryItemSector::SetOwner(TWeakObjectPtr<ACharacterBase> InCharacter)
{
	if(InCharacter.IsValid())
		OwnerCharacter = InCharacter;
}

void UInventoryItemSector::RefreshInventory(const TArray<TWeakObjectPtr<AItemActor>>& InItems)
{
	if(InItems.Num() > 50)
	{
		LOG_SCREEN(FColor::Red, TEXT("UInventory::RefreshInventory 아이템 소지 개수를 초과했습니다. %s"), *GetName())
		return;
	}
	
	if(ItemSlot.IsEmpty())
		return;
	for(int i = 0; i < InItems.Num(); ++i)
	{
		if(InItems[i] == nullptr)
				ItemSlot[i]->SetItem(nullptr);
		else
				ItemSlot[i]->SetItem(InItems[i]->GetItemImage());
	}
}
