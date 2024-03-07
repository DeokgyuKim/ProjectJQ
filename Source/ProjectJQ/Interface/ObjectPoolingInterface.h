// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>
#include "ObjectPoolingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 오브젝트 매니저 서브 시스템에서 관리되는 대상입니다.
 */
class PROJECTJQ_API IObjectPoolingInterface
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Initialize()=0;
	virtual void Release()=0;
};
