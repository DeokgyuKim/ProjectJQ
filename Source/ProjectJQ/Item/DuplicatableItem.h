// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "DuplicatableItem.generated.h"

UCLASS()
class PROJECTJQ_API ADuplicatableItem : public AItemActor
{
	GENERATED_BODY()
protected:
	int32 DuplicateCount = 1;
public:	
	// Sets default values for this actor's properties
	ADuplicatableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
