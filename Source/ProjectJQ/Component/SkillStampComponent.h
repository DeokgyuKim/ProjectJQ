// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillStampComponent.generated.h"

class ACharacterPC;
enum class ETriggerEvent : uint8;

USTRUCT(BlueprintType)
struct FSkillAnimMontageInfo
{
	GENERATED_BODY()

	//실행할 몽타주의 섹션 이름입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SectionName;

	//몽타주를 실행할 타이밍을 지정합니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ETriggerEvent PlayTiming;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTJQ_API USkillStampComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	USkillStampComponent();

	TWeakObjectPtr<ACharacterPC> OwnerPC;

	//데미지를 입힐 대상입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	ECharacterType TargetType = ECharacterType::None;

	//재생할 몽타주 정보입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	TArray<FSkillAnimMontageInfo> SkillAnimInfos;

	//충돌체 정보입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	FVector ColliderBoxExtend = FVector::ZeroVector;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UFUNCTION(BlueprintCallable)
	void ActivateSkill();

	virtual void _ActivateSkill();
	
public:
	virtual void SkillTriggered();
	virtual void SkillStarted();
	virtual void SkillOnGoing();
	virtual void SkillCanceled();
	virtual void SkillCompleted();
};
