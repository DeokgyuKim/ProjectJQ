#include "UtilEffect.h"
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>


UParticleSystemComponent* UtilEffect::SpawnParticleEffect(UWorld* InWorld, UParticleSystem* InParticleSystem,
	const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAtLocation(InWorld,InParticleSystem,
		InLocation, InRotation, InScale);

	return particleComp;
}

UParticleSystemComponent* UtilEffect::SpawnParticleEffectAttached(UParticleSystem* InParticleSystem,
	USceneComponent* InAttachComp, FName InSocketName, const FVector& InLocation, const FRotator& InRotation,
	const FVector& InScale)
{
	UParticleSystemComponent* particleComp = UGameplayStatics::SpawnEmitterAttached(InParticleSystem, InAttachComp, InSocketName,
		InLocation, InRotation, InScale, EAttachLocation::KeepRelativeOffset, true);

	return particleComp;
}

UNiagaraComponent* UtilEffect::SpawnNiagaraEffect(UWorld* InWorld, UNiagaraSystem* InNiagaraSystem,
	const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	UNiagaraComponent* niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation( InWorld, InNiagaraSystem, InLocation, InRotation );
	niagaraComp->SetWorldScale3D(InScale);

	return niagaraComp;
}

UNiagaraComponent* UtilEffect::SpawnNiagaraEffectAttached(UNiagaraSystem* InNiagaraSystem,
	USceneComponent* InAttachComp, FName InSocketName, const FVector& InLocation, const FRotator& InRotation,
	const FVector& InScale)
{
	UNiagaraComponent* niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached( InNiagaraSystem, InAttachComp, InSocketName,
		InLocation, InRotation, EAttachLocation::KeepRelativeOffset, true );
	niagaraComp->SetWorldScale3D(InScale);

	return niagaraComp;
}

void UtilEffect::SpawnSoundBase(const UObject* InWorldContextObject, USoundBase* InSound, FVector InLocation)
{
	UGameplayStatics::PlaySoundAtLocation(InWorldContextObject, InSound, InLocation);
}
