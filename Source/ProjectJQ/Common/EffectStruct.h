// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "EffectStruct.generated.h"

class UNiagaraSystem;
class UParticleSystem;
class USoundBase;

USTRUCT()
struct FEffectData
{
	GENERATED_BODY()
	
	//이펙트 재생 타입
	UPROPERTY(EditAnywhere)
	EEffectPlayType EffectPlayType;
	
	//나이아가라 이펙트
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* N_Effect = nullptr;

	//파티클시스템 이펙트
	UPROPERTY(EditAnywhere)
	UParticleSystem* Effect = nullptr;

	//사운드
	UPROPERTY(EditAnywhere)
	USoundBase* Sound = nullptr;

	//어태치 할것인가
	UPROPERTY(EditAnywhere)
	bool bAttach = false;

	//어태치 할 본 이름
	UPROPERTY(EditAnywhere)
	FName AttachBoneName = "";

	//오프셋
	UPROPERTY(EditAnywhere)
	FTransform Offset = FTransform::Identity;
};