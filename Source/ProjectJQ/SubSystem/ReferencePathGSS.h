// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "ReferencePathGSS.generated.h"

class UUserWidgetBase;

/*
 * Initailize함수 내의 넘겨받은 InPath 경로 디렉토리에 모든 에셋 래패런스 경로를 만들어내는 GameInstanceSubSystem입니다.
 * 에셋 래퍼런스 경로를 쉽게 찾는데 사용됩니다. 
 */
UCLASS()
class PROJECTJQ_API UReferencePathGSS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	TMap<FString, FString> ReferencePath;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FString GetReferencePath(FString InAssetName);
private:
    //넘겨받은 InPath 디렉토리 내의 모든 래퍼런스 경로를 찾아냅니다.
	void FindReferencePathInDirectory(FString InPath);
};
