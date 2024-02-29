// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JQAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UJQAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UJQAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	FVector Velocity; //속도

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float GroundSpeed; //xy축 기준에서의 속도

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	uint8 bIsIdle : 1; //Idle 상태인가?, bool타입은 환경마다 크기가 다르기에 언리얼 엔진에서는 uint8로 선언.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float MovingThreshold; // 해당 값 이하의 속도로 움직이면 Idle상태로 간주.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	uint8 bIsFalling : 1; // 떨어지고 있는가?

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	uint8 bIsJumping; // 점프상태인가?

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float JumpingThreshold; // 현재 점프중인지를 나타내기 위한 값.
};
