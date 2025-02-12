// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "JQWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTJQ_API UJQWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void InitWidget() override;
};
