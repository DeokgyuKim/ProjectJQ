// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "DecoratorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJQ_API UDecoratorComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	//오너 클래스
	TWeakObjectPtr<AActor> Owner;

	//기본적으로 재생할 이펙트들을 답아줍니다.
	UPROPERTY(EditDefaultsOnly, Category="JQ_Setting")
	TArray<FEffectData> EffectDatas;

public:	
	// Sets default values for this component's properties
	UDecoratorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void StartEffect(EEffectPlayType InEffectPlayType, FName InBoneName = "");
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category="JQ_Effect")
	void PlayWalkEffect(FName InBoneName);
};
