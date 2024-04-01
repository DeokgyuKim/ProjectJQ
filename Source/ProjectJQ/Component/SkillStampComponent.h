// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillStampComponent.generated.h"

class ACharacterPC;
class AJQProjectile;
enum class ETriggerEvent : uint8;

USTRUCT(BlueprintType)
struct FSkillAnimMontageInfo
{
	GENERATED_BODY()

	//실행할 몽타주의 섹션 이름입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SectionName;

	// 몽타주 콤보 갯수입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ComboCount;

	//몽타주를 실행할 타이밍을 지정합니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ETriggerEvent PlayTiming;

	//Projectile Section;

	//투사체 파티클(케스케이드)을 저장합니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> EmitterSystem;

	//투사체 파티클(나이아가라)을 저장합니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTJQ_API USkillStampComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	USkillStampComponent();

	TWeakObjectPtr<ACharacterPC> OwnerPC;

	// 현재 몇 번째 콤보가 재생되고 있는지 저장하는 값입니다.
	int32 Count;

	//데미지를 입힐 대상입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	ECharacterType TargetType = ECharacterType::None;

	//재생할 몽타주 정보입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	TArray<FSkillAnimMontageInfo> SkillAnimInfos;
	
	//충돌체 정보입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	FVector ColliderBoxExtend = FVector::ZeroVector;
	
	// 트리거 이벤트와 몽타주 섹션 이름을 담아두는 맵입니다.
	TMap<ETriggerEvent, FSkillAnimMontageInfo> EventSkillsMap;

	//투사체 BP를 저장하는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AJQProjectile> ProjectileObject;
	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateSkill(ETriggerEvent InEvent, EAttackRangeType InAttackRange);
	virtual void _ActiveSkill(ETriggerEvent InEvent, EAttackRangeType InAttackRange);
	
	virtual void ActiveBoxCollisionAttack(ETriggerEvent InEvent);
	virtual void ActiveSphereCollsionAttack(ETriggerEvent InEvent);
	virtual void ActiveProjectileAttack(ETriggerEvent InEvent);
public:
	virtual void SkillTriggered();
	virtual void SkillStarted();
	virtual void SkillOnGoing();
	virtual void SkillCanceled();
	virtual void SkillCompleted();
};
