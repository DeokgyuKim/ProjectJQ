// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillStampComponent.generated.h"

class ACharacterPC;
class AJQProjectile;
class UMaterialInterface;
class ADecalActor;
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
	
	//Box 공격 형태 충돌체 정보입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	FVector ColliderBoxExtend = FVector::ZeroVector;

	/*
	 공격 범위 길이입니다.
	 box : 해당 변수를 사용하지 않음.
	 sphere : 구의 반지름
	 Projectile : 투사체 날아가는 거리
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	float Length;
	
	// 트리거 이벤트와 몽타주 섹션 이름을 담아두는 맵입니다.
	TMap<ETriggerEvent, FSkillAnimMontageInfo> EventSkillsMap;

	//투사체 BP를 저장하는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JQ_Skill")
	TSubclassOf<AJQProjectile> ProjectileObject;

	//충돌 판정을 어떤 걸로 할 건지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JQ_Skill")
	EAttackRangeType AttackRangeType = EAttackRangeType::None;

	//데칼 머테리얼, null이 아니면 키 Released 시에 스킬이 시전됩니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	TObjectPtr<UMaterialInterface> DecalMaterial;

	//데칼 액터 포인터
	TObjectPtr<ADecalActor> DecalActor;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateSkill(FName InAnimSectionName);
	virtual void _ActiveSkill(FName InAnimSectionName);
	
	virtual void ActiveBoxCollisionAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo);
	virtual void ActiveSphereCollsionAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo);
	virtual void ActiveProjectileAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo);
public:
	virtual void SkillStarted();
	virtual void SkillTriggered();
	virtual void SkillOnGoing();
	virtual void SkillCanceled();
	virtual void SkillCompleted();

protected:
	//카메라에서 마우스 커서로 레이를 쏴서 가장 처음 부딫힌 지점의 월드 좌표계를 구합니다.
	FVector GetWorldLocationAtMousePointer();
	//캐릭터에서 마우스 커서를 향하는 정규 벡터를 구합니다.
	FVector GetVector2DFromCharacterToMousePointer();
	//캐릭터를 마우스 방향으로 쳐다보게 합니다.
	void SetCharacterRotationToMousePointer(FVector InVectorToMouse) const;
};
