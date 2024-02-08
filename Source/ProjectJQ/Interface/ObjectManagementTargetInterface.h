// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>
#include "ObjectManagementTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectManagementTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 오브젝트 매니저 서브 시스템에서 관리되는 대상입니다.
 */
class PROJECTJQ_API IObjectManagementTargetInterface
{
	GENERATED_BODY()

	int32 ObjectId = -1;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void SetObjectId(int32 InObjectId){ObjectId = InObjectId;}
	const int32& GetObjectId() const {return ObjectId;} 
};
