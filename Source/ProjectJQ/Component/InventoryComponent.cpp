// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ProjectJQ/Character/CharacterBase.h"
#include "ProjectJQ/Data/ItemDataTable.h"
#include "ProjectJQ/Item/ItemActor.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"
#include "ProjectJQ/SubSystem/UIManagementGSS.h"
#include "ProjectJQ/UI/Inventory.h"

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

	UDataTable* table = LOAD_DATA_TABLE(Item);
	if(table == nullptr)
		return;

	if(GetOwner() == nullptr)
		return;

	//PresetItemNames를 기반으로 미리 세팅된 아이템 생성
	for(const FString& name : PresetItemNames)
	{
		FItemDataTable* data = table->FindRow<FItemDataTable>(*name, name);
		if(data == nullptr)
			continue;
		
		UObjectManagementGSS* gss = GetOwner()->GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
		FSpawnParam spawnParam;
		spawnParam.Location = FVector::ZeroVector;
		spawnParam.Rotation = FRotator::ZeroRotator;
		TWeakObjectPtr<AActor> ownerPtr = GetOwner();
		spawnParam.CallbackSpawn = [ownerPtr, &name](AActor* InActor)
		{
			if(ownerPtr.IsValid())
				Cast<AItemActor>(InActor)->SetItemOwner(ownerPtr);
			Cast<AItemActor>(InActor)->SetItemName(name);
		};
		AItemActor* item = gss->CreateActor<AItemActor>(AItemActor::StaticClass(), spawnParam);
		if(item != nullptr)
			Items.Add(item);
	}

	UUIManagementGSS* gss = GetOwner()->GetGameInstance()->GetSubsystem<UUIManagementGSS>();
	int id = gss->CreateWidgetBase(TEXT("WBP_Inventory"), GetOwner()->GetName() + TEXT("_WBP_Inventory"), GetOwner());
	if(id == UUIManagementGSS::INVALID_UIID)
	{
		LOG_SCREEN(FColor::Red, TEXT("Inventory UI 생성 실패: %d"), id)
		return;
	}

	InventoryUI = Cast<UInventory>(gss->FindWidgetBase(id));
	InventoryUI->SetOwner(Cast<ACharacterBase>(GetOwner()));
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
		item->SetActorHiddenInGame(false);
		
		item->SetActorLocation(GetOwner()->GetActorLocation());
	}
}
