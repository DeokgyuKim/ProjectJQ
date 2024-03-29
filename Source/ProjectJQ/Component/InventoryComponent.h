// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "InventoryComponent.generated.h"

class UInventory;
class AItemActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJQ_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	//미리 들고있을 아이템을 설정합니다. Item 데이터 테이블을 바탕으로 만들어집니다.
	UPROPERTY(EditDefaultsOnly, Category="JQ_Item")
	TArray<FString> PresetItemNames;
	
	//인벤토리 아이템
	TArray<TWeakObjectPtr<AItemActor>> Items;

	//골드
	UPROPERTY(EditDefaultsOnly, Category="JQ_Item")
	int32 Gold;

	UPROPERTY()
	TObjectPtr<UInventory> InventoryUI;
	
public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginDestroy() override;

public:
	UInventory* GetInventoryUI(){return InventoryUI;}
};
