// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillStampComponent.generated.h"

class ACharacterPC;
class ACpProjectile;
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

USTRUCT(BlueprintType)
struct FBoxInfo
{
	GENERATED_BODY()

	//공격 박스 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "JQ_Skill")
	FVector AttackBox;
};

USTRUCT(BlueprintType)
struct FSphereInfo
{
	GENERATED_BODY()

	//구의 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "JQ_Skill")
	float SphereRadius;
};

USTRUCT(BlueprintType)
struct FArcInfo
{
	GENERATED_BODY()

	//호의 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "JQ_Skill")
	float Radius;

	//호의 각도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "JQ_Skill")
	float ArcAngle;
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	//단발인지, 여러발 나가는지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	bool IsSingleShot;

	//여러발이면 몇 개가 발사되는지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	int NumOfProjectile;

	//여러발이 어느 각도로 펼쳐 발사되는지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	int Angle;

	//투사체의 발사 범위
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JQ_Skill")
	TSubclassOf<ACpProjectile> ProjectileObject;
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

	// 트리거 이벤트와 몽타주 섹션 이름을 담아두는 맵입니다.
	TMap<ETriggerEvent, FSkillAnimMontageInfo> EventSkillsMap;

	//충돌 판정을 어떤 걸로 할 건지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JQ_Skill")
	EAttackRangeType AttackRangeType = EAttackRangeType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	FBoxInfo BasicAttackInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	FSphereInfo SphereAttackInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	FArcInfo ArcAttackInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JQ_Skill")
	FProjectileInfo ProjectileAttackInfo;

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
	virtual void ActiveArcAttack(FSkillAnimMontageInfo* InCurrentPlayAnimMontageInfo);

	FVector RotateVectorFromYaw(FVector InVector, float Degree);
};
