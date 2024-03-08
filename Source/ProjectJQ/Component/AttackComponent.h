// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJQ_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= JQ_Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= JQ_Attack, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UJQComboActionData> ComboActionData;

	int32 CurrentCombo = 0; //0 : 콤보가 시작되지 않음, n : n번째 타수 진행 중
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false; // 발동한 타이머 이전에 입력 커멘드가 들어왔는지 점검하는 필드값.

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer(); // 타이머 발동
	void ComboCheck();			// 타이머가 발동 되었으면 입력이 들어왔는지 체크

public:
	void ProcessComboCommand();

	void AttackHitCheck();
};
