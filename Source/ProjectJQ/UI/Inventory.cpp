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

void UInventory::OnCreated()
{
	Super::OnCreated();

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

void UInventory::RefreshInventory(const TArray<TWeakObjectPtr<AItemActor>>& InItems)
{
	if(InItems.Num() > 50)
	{
		LOG_SCREEN(FColor::Red, TEXT("UInventory::RefreshInventory 아이템 소지 개수를 초과했습니다. %s"), *GetName())
		return;
	}
	ItemSector->RefreshInventory(InItems);
}
