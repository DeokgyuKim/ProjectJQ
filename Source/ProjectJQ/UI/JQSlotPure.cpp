// Fill out your copyright notice in the Description page of Project Settings.


#include "JQSlotPure.h"

#include "Inventory.h"
#include "SlotDragDropOper.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "GameFramework/Pawn.h"
#include "ProjectJQ/Component/InventoryComponent.h"
#include "ProjectJQ/Item/ItemActor.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"

void UJQSlotPure::OnCreated()
{
	Super::OnCreated();
	if(BasicTexture)
		Background->SetBrushFromTexture(BasicTexture);
	ItemImage->SetOpacity(0.f);
}

void UJQSlotPure::SetItem(int32 InItemID)
{
	UObjectManagementGSS* gss = GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
	if(!gss)
		return;

	ItemId = InItemID;
	AItemActor* item = Cast<AItemActor>(gss->FindActor(InItemID));

	if(item)
	{
		UTexture2D* texture = item->GetItemImage();
		ItemImage->SetOpacity(1.f);
		ItemImage->SetBrushFromTexture(texture);
	}
	else
	{
		ItemImage->SetOpacity(0.f);
		ItemImage->SetBrushFromTexture(nullptr);
	}
	
}

void UJQSlotPure::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	//LOG_SCREEN(FColor::White, TEXT("Mouse Enter %s"), *InMouseEvent.GetEffectingButton().ToString())

	if(HoveringTexture)
		Background->SetBrushFromTexture(HoveringTexture);
}

void UJQSlotPure::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	//LOG_SCREEN(FColor::White, TEXT("Mouse Leave %s"), *InMouseEvent.GetEffectingButton().ToString())

	if(BasicTexture)
		Background->SetBrushFromTexture(BasicTexture);
}

FReply UJQSlotPure::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//LOG_SCREEN(FColor::White, TEXT("Mouse Button %s Down"), *InMouseEvent.GetEffectingButton().ToString())

	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if(ItemId >= 0)
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}
	
	return reply.NativeReply;
}

FReply UJQSlotPure::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//LOG_SCREEN(FColor::White, TEXT("Mouse Button %s Up"), *InMouseEvent.GetEffectingButton().ToString())
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UJQSlotPure::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if(OutOperation == nullptr)
	{
		LOG_SCREEN(FColor::White, TEXT("DragStart : %i"), GetSlotIndex())

		USlotDragDropOper* oper = NewObject<USlotDragDropOper>();
		OutOperation = oper;
		oper->SlotIndex = SlotIndex;
		oper->ItemId = ItemId;
		oper->DefaultDragVisual = ItemImage;
	}
	
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UJQSlotPure::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if(USlotDragDropOper* slotOper = Cast<USlotDragDropOper>(InOperation))
	{
		if(IsCanSwap(slotOper))
		{
			LOG_SCREEN(FColor::White, TEXT("DragComplete : %d"), GetSlotIndex())
			if(UInventoryComponent* inventoryComp = Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass())))
			{
				inventoryComp->SwapItem(slotOper->SlotIndex, SlotIndex);
				return true;
			}
		}
		else
			return true;
	}
	
	LOG_SCREEN(FColor::White, TEXT("DragFailed : %d"), GetSlotIndex())
	return false;
}
