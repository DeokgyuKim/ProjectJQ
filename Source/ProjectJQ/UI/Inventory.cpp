// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "CpButtonList.h"
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
#include "JQEquipSlot.h"
#include "../SubSystem/ObjectManagementGSS.h"
#include "Components/CanvasPanelSlot.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SViewport.h"

void UInventory::OnCreated()
{
	Super::OnCreated();
	
	EquipSlot.FindOrAdd(EEquipItemUIType::Helmet) = Slot_Helmet;
	EquipSlot.FindOrAdd(EEquipItemUIType::Armor) = Slot_Armor;
	EquipSlot.FindOrAdd(EEquipItemUIType::LWeapon) = Slot_LWeapon;
	EquipSlot.FindOrAdd(EEquipItemUIType::RWeapon) = Slot_RWeapon;
	EquipSlot.FindOrAdd(EEquipItemUIType::Shoes) = Slot_Shoes;
	EquipSlot.FindOrAdd(EEquipItemUIType::Necklace) = Slot_Necklace;

	for(EEquipItemUIType itemType : TEnumRange<EEquipItemUIType>())
	{
		if(EquipSlot[itemType])
			EquipSlot[itemType]->OnCreated();
	}

	ItemSector->OnCreated();
	TArray<TWeakObjectPtr<UJQSlotPure>>& slots = ItemSector->GetItemSlots();
	for(TWeakObjectPtr<UJQSlotPure>& slot : slots)
	{
		slot->BindUFunctionOnDelegateRightButtonDown(this, FName(TEXT("RightButtonClickedSlot")));
	}
	
	SlotButtonList->OnCreated();
	SlotButtonList->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventory::SetOwner(TWeakObjectPtr<ACharacterBase> InCharacter)
{
	if(InCharacter.IsValid())
		OwnerCharacter = InCharacter;
}

void UInventory::RefreshInventory(const TMap<EEquipItemUIType, TWeakObjectPtr<AItemActor>> InEquipItems, const TArray<TWeakObjectPtr<AItemActor>>& InItems)
{
	if(InItems.Num() > 50)
	{
		LOG_SCREEN(FColor::Red, TEXT("UInventory::RefreshInventory 아이템 소지 개수를 초과했습니다. %s"), *GetName())
		return;
	}

	for(EEquipItemUIType itemType : TEnumRange<EEquipItemUIType>())
	{
		if(InEquipItems[itemType].IsValid())
			EquipSlot[itemType]->SetItem(InEquipItems[itemType]->GetObjectId());
		else
			EquipSlot[itemType]->SetItem();
	}
	
	ItemSector->RefreshInventory(InItems);
}

void UInventory::RightButtonClickedSlot(UJQSlotPure* InSlot, int32 InItemID)
{
	SlotButtonList->SetVisibility(ESlateVisibility::Visible);
	
	UObjectManagementGSS* gss = GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
	if(!gss)
		return;

	AItemActor* item = Cast<AItemActor>(gss->FindActor(InItemID));
	if(!item)
	{
		SlotButtonList->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	ClickedItem = item;

	SlotButtonListTexts.Empty();
	if(Cast<UJQEquipSlot>(InSlot))		//슬롯이 장비 아이템인 경우
	{
		switch (item->GetItemType())	//아이템의 타입
		{
		case EItemType::Equip:
			SlotButtonListTexts.Add(TEXT("자세히보기"));
			SlotButtonListTexts.Add(TEXT("해제"));
			SlotButtonListTexts.Add(TEXT("버리기"));
			break;
		case EItemType::Gemstone:
			break;
		case EItemType::Quest:
			break;
		}
	}
	else								//일반 인벤토리 슬롯의 경우
	{
		switch (item->GetItemType())
		{
		case EItemType::Equip:
			SlotButtonListTexts.Add(TEXT("자세히보기"));
			SlotButtonListTexts.Add(TEXT("장착"));
			SlotButtonListTexts.Add(TEXT("버리기"));
			break;
		case EItemType::Gemstone:
			SlotButtonListTexts.Add(TEXT("자세히보기"));
			SlotButtonListTexts.Add(TEXT("버리기"));
			break;
		case EItemType::Quest:
			SlotButtonListTexts.Add(TEXT("자세히보기"));
			SlotButtonListTexts.Add(TEXT("버리기"));
			break;
		}
	}
	
	SlotButtonList->SetButtonTexts(SlotButtonListTexts);
	SlotButtonList->SetVisibility(ESlateVisibility::Visible);
	SlotButtonList->ClearDelegateCpButtonListOnClicked();
	SlotButtonList->BindUFunctionDelegateCpButtonListOnClicked(this, FName(TEXT("OnClickedSlotButtonList")));

	//todo: SetPositionInViewport 사용하는 것 공용 로직으로 만들기
	/*
	FVector2D pos2DResult = InSlot->GetCachedGeometry().GetAbsolutePosition();
	FVector2D viewportAbsolutePos = FSlateApplication::Get().GetGameViewport().Get()->GetCachedGeometry().GetAbsolutePosition();
	float viewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	FVector2D pos = (pos2DResult - viewportAbsolutePos) / viewportScale;
	LOG_SCREEN(FColor::Red, TEXT("%f, %f"), pos2DResult.X, pos2DResult.Y)
	LOG_SCREEN(FColor::Green, TEXT("%f, %f"), viewportAbsolutePos.X, viewportAbsolutePos.Y)
	LOG_SCREEN(FColor::Blue, TEXT("%f"), viewportScale)
	
	SlotButtonList->SetPositionInViewport(pos);
	*/
	
	//UCanvasPanelSlot* panel = Cast<UCanvasPanelSlot>(SlotButtonList->Slot);
	//panel->SetPosition()
}

void UInventory::OnClickedSlotButtonList(int32 InButtonIndex)
{
	if(!ClickedItem.IsValid())
	{
		LOG_SCREEN(FColor::Red, TEXT("선택된 아이템이 없습니다. 행동: %d"), InButtonIndex);
		return;
	}
	
	if(InButtonIndex >= SlotButtonListTexts.Num())
	{
		LOG_SCREEN(FColor::Red, TEXT("눌린 버튼의 인덱스가 지정해야되는 행동보다 크거나 같습니다.! ButtonIndex: %d, SlotButtonListTexts: %d"), InButtonIndex, SlotButtonListTexts.Num());
		SlotButtonList->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if(SlotButtonListTexts[InButtonIndex] == TEXT("자세히보기"))
	{
		LOG_SCREEN(FColor::White, TEXT("%s 아이템을 자세히 봅니다."), *ClickedItem->GetItemName())
	}
	else if(SlotButtonListTexts[InButtonIndex] == TEXT("장착"))
	{
		LOG_SCREEN(FColor::White, TEXT("%s 아이템을 장착합니다."), *ClickedItem->GetItemName())
	}
	else if(SlotButtonListTexts[InButtonIndex] == TEXT("해제"))
	{
		LOG_SCREEN(FColor::White, TEXT("%s 아이템을 해제합니다."), *ClickedItem->GetItemName())
	}
	else if(SlotButtonListTexts[InButtonIndex] == TEXT("버리기"))
	{
		LOG_SCREEN(FColor::White, TEXT("%s 아이템을 버립니다."), *ClickedItem->GetItemName())
	}
	
	SlotButtonList->SetVisibility(ESlateVisibility::Collapsed);
}
