// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DuplicatableItem.h"
#include "QuestItem.generated.h"

UCLASS()
class PROJECTJQ_API AQuestItem : public ADuplicatableItem
{
	GENERATED_BODY()
protected:
	
public:	
	// Sets default values for this actor's properties
	AQuestItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
