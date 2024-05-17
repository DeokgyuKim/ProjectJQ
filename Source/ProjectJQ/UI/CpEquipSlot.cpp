// Fill out your copyright notice in the Description page of Project Settings.


#include "CpEquipSlot.h"

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
#include "ProjectJQ/Item/EquipItem.h"
#include "ProjectJQ/Item/ItemActor.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"

bool UCpEquipSlot::IsCanSwap(USlotDragDropOper* InSlotOper)
{
	//상위 객체의 조건에 해당하는 가?
	bool precondition = Super::IsCanSwap(InSlotOper);

	//현재 객체의 조건에 해당하는 가?
	bool condition = false;

	UObjectManagementGSS* gss = GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
	if(!gss)
		return false;

	AEquipItem* item = Cast<AEquipItem>(gss->FindActor(InSlotOper->ItemId));
	if(item)
		if(EquipItemType == item->GetEquipItemType())
			condition = true;
	
	return precondition && condition;
}

void UCpEquipSlot::OnCreated()
{
	Super::OnCreated();
}
