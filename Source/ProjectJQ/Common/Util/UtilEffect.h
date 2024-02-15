#pragma once

//이펙트 재생을 위한 유틸 함수 모음입니다.

class USceneComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundBase;

namespace UtilEffect
{
	// 파티클 이펙트를 생성한다.
	UParticleSystemComponent* SpawnParticleEffect(UWorld* InWorld, UParticleSystem* InParticleSystem, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale);

	// 파티클 이펙트를 생성한다.(Attached)
	UParticleSystemComponent* SpawnParticleEffectAttached(UParticleSystem* InParticleSystem, USceneComponent* InAttachComp, FName InSocketName = NAME_None,
		const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1.f, 1.f, 1.f));
	
	// 나이아가라 이펙트를 생성한다.
	UNiagaraComponent* SpawnNiagaraEffect(UWorld* InWorld, UNiagaraSystem* InNiagaraSystem, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale);

	// 나이아가라 이펙트를 생성한다.(Attached)
	UNiagaraComponent* SpawnNiagaraEffectAttached( UNiagaraSystem* InNiagaraSystem, USceneComponent* InAttachComp, FName InSocketName = NAME_None,
		const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1.f, 1.f, 1.f));

	//사운드 베이스를 재생한다.
	void SpawnSoundBase(const UObject* InWorldContextObject, USoundBase* InSound, FVector InLocation);
}