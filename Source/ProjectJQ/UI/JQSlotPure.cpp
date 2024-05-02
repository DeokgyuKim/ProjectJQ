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

void UJQSlotPure::OnCreated()
{
	Super::OnCreated();
	if(BasicTexture)
		Background->SetBrushFromTexture(BasicTexture);
	ItemImage->SetOpacity(0.f);
}

void UJQSlotPure::SetItem(UTexture2D* InItemImage)
{
	ItemTexture = InItemImage;
	if(InItemImage == nullptr)
		ItemImage->SetOpacity(0.f);		
	else
		ItemImage->SetOpacity(1.f);
	ItemImage->SetBrushFromTexture(InItemImage);
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
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	//LOG_SCREEN(FColor::White, TEXT("DragDetected Key: %s"), *InMouseEvent.GetEffectingButton().ToString())

	if(OutOperation == nullptr)
	{
		LOG_SCREEN(FColor::White, TEXT("DragStart : %i"), GetSlotIndex())

		USlotDragDropOper* oper = NewObject<USlotDragDropOper>();
		OutOperation = oper;
		oper->SlotIndex = SlotIndex;
		oper->DefaultDragVisual = ItemImage;
	}
}

bool UJQSlotPure::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if(USlotDragDropOper* slotOper = Cast<USlotDragDropOper>(InOperation))
	{
		LOG_SCREEN(FColor::White, TEXT("DragComplete : %d"), GetSlotIndex())
		if(UInventoryComponent* inventoryComp = Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass())))
		{
			inventoryComp->SwapItem(slotOper->SlotIndex, SlotIndex);
			return true;
		}
	}
	
	LOG_SCREEN(FColor::White, TEXT("DragFailed : %d"), GetSlotIndex())
	return false;
}
