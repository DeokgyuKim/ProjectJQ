// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDragDropOper.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJQ_API USlotDragDropOper : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SlotIndex;
};
