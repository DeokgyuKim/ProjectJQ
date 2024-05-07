// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ProjectJQ/Character/CharacterBase.h"
#include "ProjectJQ/Data/ItemDataTable.h"
#include "ProjectJQ/Item/ItemActor.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"
#include "ProjectJQ/SubSystem/UIManagementGSS.h"
#include "ProjectJQ/UI/Inventory.h"
#include "ProjectJQ/UI/JQSlotPure.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner() == nullptr)
		return;
	
	UUIManagementGSS* uiGss = GetOwner()->GetGameInstance()->GetSubsystem<UUIManagementGSS>();
	int id = uiGss->CreateWidgetBase(TEXT("WBP_Inventory"), GetOwner()->GetName() + TEXT("_WBP_Inventory"), GetOwner());
	if(id == UUIManagementGSS::INVALID_UIID)
	{
		LOG_SCREEN(FColor::Red, TEXT("Inventory UI 생성 실패: %d"), id)
		return;
	}

	InventoryUI = Cast<UInventory>(uiGss->FindWidgetBase(id));
	InventoryUI->SetOwner(Cast<ACharacterBase>(GetOwner()));


	UDataTable* table = LOAD_DATA_TABLE(Item);
	if(table == nullptr)
		return;

	Items.SetNum(TotalItemCount);
	
	//PresetItemNames를 기반으로 미리 세팅된 아이템 생성
	for(int i = 0; i < PresetItemNames.Num(); ++i)
	{
		FItemDataTable* data = table->FindRow<FItemDataTable>(*PresetItemNames[i], PresetItemNames[i]);
		if(data == nullptr)
			continue;
		
		UObjectManagementGSS* omGss = GetOwner()->GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
		FSpawnParam spawnParam;
		spawnParam.Location = FVector::ZeroVector;
		spawnParam.Rotation = FRotator::ZeroRotator;
		TWeakObjectPtr<AActor> ownerPtr = GetOwner();
		FString name = PresetItemNames[i];
		spawnParam.CallbackSpawn = [ownerPtr, name](AActor* InActor)
		{
			if(ownerPtr.IsValid())
				Cast<AItemActor>(InActor)->SetItemOwner(ownerPtr);
			Cast<AItemActor>(InActor)->SetItemName(name);
		};
		AItemActor* item = omGss->CreateActor<AItemActor>(AItemActor::StaticClass(), spawnParam);
		if(item != nullptr)
		{
			Items[i] = item;
			//FString slotName = FString::Printf(TEXT("ItemSlot_%d"), i);
			//UWidget* widget = (*InventoryUI)[*slotName];
			//if(UJQSlotPure* slot = Cast<UJQSlotPure>(widget))
			//	slot->SetItem(item->GetItemImage());
		}
		
		if(i >= 50)
			break;
	}
	
	for(EEquipItemUIType itemType : TEnumRange<EEquipItemUIType>())
			EquipItems.FindOrAdd(itemType) = nullptr;

	InventoryUI->RefreshInventory(EquipItems, Items);
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::BeginDestroy()
{
	Super::BeginDestroy();

	for(TWeakObjectPtr<AItemActor>& item : Items)
	{
		//item->SetActorHiddenInGame(false);
		
		//item->SetActorLocation(GetOwner()->GetActorLocation());
	}
}

void UInventoryComponent::SwapItem(int32 InFromIndex, int32 InToIndex)
{
	//교체할 아이템
	TWeakObjectPtr<AItemActor> fromItem = nullptr;
	if(InFromIndex >= TotalItemCount)
	{
		fromItem = EquipItems[static_cast<EEquipItemUIType>(InFromIndex - TotalItemCount)];
	}
	else
		fromItem = Items[InFromIndex];
	
	TWeakObjectPtr<AItemActor> toItem = nullptr;
	if(InToIndex >= TotalItemCount)
	{
		toItem = EquipItems[static_cast<EEquipItemUIType>(InToIndex - TotalItemCount)];
	}
	else
		toItem = Items[InToIndex];

	//교체할 슬롯
	if(InFromIndex >= TotalItemCount)
	{
		if(EquipItems[static_cast<EEquipItemUIType>(InFromIndex - TotalItemCount)].IsValid())
			EquipItems[static_cast<EEquipItemUIType>(InFromIndex - TotalItemCount)]->OnUnPossess(GetOwner());
		EquipItems[static_cast<EEquipItemUIType>(InFromIndex - TotalItemCount)] = toItem;
		if(EquipItems[static_cast<EEquipItemUIType>(InFromIndex - TotalItemCount)].IsValid())
			EquipItems[static_cast<EEquipItemUIType>(InFromIndex - TotalItemCount)]->OnPossess(GetOwner());
	}
	else
		Items[InFromIndex] = toItem;

	if(InToIndex >= TotalItemCount)
	{
		if(EquipItems[static_cast<EEquipItemUIType>(InToIndex - TotalItemCount)].IsValid())
			EquipItems[static_cast<EEquipItemUIType>(InToIndex - TotalItemCount)]->OnUnPossess(GetOwner());
		EquipItems[static_cast<EEquipItemUIType>(InToIndex - TotalItemCount)] = fromItem;
		if(EquipItems[static_cast<EEquipItemUIType>(InToIndex - TotalItemCount)].IsValid())
			EquipItems[static_cast<EEquipItemUIType>(InToIndex - TotalItemCount)]->OnPossess(GetOwner());
	}
	else
		Items[InToIndex] = fromItem;
	

	InventoryUI->RefreshInventory(EquipItems, Items);
}
