// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "StatControlComponent.generated.h"

class FBuff;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJQ_API UStatControlComponent : public UActorComponent
{
	GENERATED_BODY()
	
	//스탯 변경 시 호출될 이벤트
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FSetStatDelegate, TWeakObjectPtr<AActor>, EStatControlType, double)
protected:
	// 액터의 이름을 지정합니다. 이 이름을 기반으로 데이터 테이블을 검색하므로 꼭 데이터를 채워야합니다.
	UPROPERTY(EditDefaultsOnly, Category="JQ_EssentialSetting")
	FString ActorName = TEXT("");
	
	//오너 클래스
	TWeakObjectPtr<AActor> Owner;

	//스탯 정보
	TMap<EStatControlType, double> Stat;

	//스탯 변경 시 호출될 델리게이트
	FSetStatDelegate DelegateChangeStat;

	TMap<FString, FBuff*> Buffs;
public:
	constexpr static double INVALID_STAT = -999999.0;
public:	
	// Sets default values for this component's properties
	UStatControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ChangeStatDelegateFunction(TWeakObjectPtr<AActor> InActor, EStatControlType InStatType, double InValue);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetStat(EStatControlType InStatType, double InValue);

	const double& GetStat(EStatControlType InStatType);

	void AddBuff(const FString& InBuffName);

	FSetStatDelegate& GetSetStatDelegate(){return DelegateChangeStat;}
};
