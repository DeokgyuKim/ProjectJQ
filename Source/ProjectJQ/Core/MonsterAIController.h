// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "MonsterAIController.generated.h"

UCLASS()
class PROJECTJQ_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

private:
	// 비헤비어 트리
	UPROPERTY(EditAnywhere, Category = "JQ_Setting")
	UBehaviorTree* AIBehavior;

public:
	static const FName TargetKey;		//블랙보드에 등록된 변수를 지칭합니다.
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
protected:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval = 5.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Ai를 실행합니다.
	void RunAi(APawn* inPawn, bool inForced = false);

	// 소유한 Actor Getter
	class ACharacterMonster* GetMonster();
};
