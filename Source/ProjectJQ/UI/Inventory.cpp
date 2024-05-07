// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

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
