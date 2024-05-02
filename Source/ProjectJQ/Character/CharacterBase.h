// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "../Interface/ObjectManagementTargetInterface.h"
#include "CharacterBase.generated.h"

class UDecoratorComponent;
class UStatControlComponent;
class UAttackComponent;
class UAnimMontage;

UCLASS(Blueprintable)
class ACharacterBase : public ACharacter, public IObjectManagementTargetInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatControlComponent> StatControlComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecoratorComponent> DecoratorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttackComponent> AttackComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="JQ_Animation")
	TObjectPtr<UAnimMontage> AnimMontage;

	ECharacterType CharacterType = ECharacterType::None;

	// true이면 공격 불가능 (ex. 스킬 애니메이션이 재생 중일 때에는 스킬 입력을 막는다)
	bool CanAttack = true;
public:
	constexpr static float INVALID_ANIMMONTAGE = -1.f;
public:
	ACharacterBase();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

public:
	ECharacterType GetCharacterType() const {return CharacterType;}

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//캐릭터 베이스에 등록한 AnimMontage를 재생합니다.
	float PlayCharacterAnimMontage(float InPlayRate, FName InSectionName);

	FORCEINLINE void SetCanAttack(bool InCanAttack) {CanAttack = InCanAttack;}
	FORCEINLINE bool GetCanAttack() const {return CanAttack;}
};

