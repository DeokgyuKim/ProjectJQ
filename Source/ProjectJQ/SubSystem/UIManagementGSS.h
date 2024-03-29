// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "UIManagementGSS.generated.h"

class UUserWidgetBase;

/*
 * UI를 관리하는 GameInstanceSubSystem입니다. UserWidgetBase를 상속받은 위젯들을 관리합니다.
 * ObjectManagementTargetInterface를 상속받은 액터를 생성, 보관, 검색, 삭제의 기능을 제공합니다. 
 */
UCLASS()
class PROJECTJQ_API UUIManagementGSS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	TMap<int32, TWeakObjectPtr<UUserWidgetBase>>	ManagementTargets;
	FIdGenerator<int32>								UIIdGenerator;
	TWeakObjectPtr<UWorld>							World;
	TWeakObjectPtr<APlayerController>				Controller;
	TMap<FString, FString>							UIReferencePath;

public:
	static constexpr int32 INVALID_UIID = -9999;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void SetController(TWeakObjectPtr<APlayerController> InController){Controller = InController;}
public:
	int32 CreateWidgetBase(FString InBPName, FString InWidgetName, AActor* InOwner, UUserWidgetBase* InOwningObject = nullptr);
	int32 CreateWidgetBase(UClass* InUClass, FString InWidgetName, AActor* InOwner, UUserWidgetBase* InOwningObject = nullptr);
	void DestroyWidgetBase(int32 InUserWidgetId);
	UUserWidgetBase* FindWidgetBase(int32 InUserWidgetId);
	UClass* GetUClassWidgetBP(FString InBPName);
private:
	void FindUIDirectroy(FString InPath, TArray<FString>& OutData);
};
