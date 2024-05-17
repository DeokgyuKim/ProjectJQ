// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CpAnimInstance.h"
#include "CpMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJQ_API UCpMonsterAnimInstance : public UCpAnimInstance
{
	GENERATED_BODY()
	
public:
	UCpMonsterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
