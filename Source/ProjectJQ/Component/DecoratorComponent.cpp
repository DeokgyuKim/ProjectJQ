// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoratorComponent.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UDecoratorComponent::UDecoratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDecoratorComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if(!Owner.IsValid())
		return;

	StartEffect(EEffectPlayType::Spawn);
}

void UDecoratorComponent::StartEffect(EEffectPlayType InEffectPlayType, FName InBoneName)
{
	for(const FEffectData& effect : EffectDatas)
	{
		if(effect.EffectPlayType != InEffectPlayType)
			continue;

		if(effect.bAttach)
		{
			if(InBoneName != "" && effect.AttachBoneName != InBoneName)
				continue;
			
			TArray<USkeletalMeshComponent*> comp;
			Owner->GetComponents<USkeletalMeshComponent>(comp);

			USkeletalMeshComponent* mesh = nullptr;

			for(int i = 0; i < comp.Num(); ++i)
			{
				TArray<FName> names;
				comp[i]->GetBoneNames(names);

				for(const FName& name : names)
				{
					if(name == effect.AttachBoneName)
					{
						mesh = comp[i];
						break;
					}
				}
			}

			if(mesh == nullptr)
				return;
			
			if (effect.N_Effect)
				UtilEffect::SpawnNiagaraEffectAttached(effect.N_Effect, mesh, effect.AttachBoneName, effect.Offset.GetLocation(), effect.Offset.GetRotation().Rotator(), effect.Offset.GetScale3D());
			if (effect.Effect)
				UtilEffect::SpawnParticleEffectAttached(effect.Effect, mesh, effect.AttachBoneName, effect.Offset.GetLocation(), effect.Offset.GetRotation().Rotator(), effect.Offset.GetScale3D());
			if (effect.Sound)
				UtilEffect::SpawnSoundBase(this, effect.Sound, mesh->GetBoneLocation(effect.AttachBoneName, EBoneSpaces::WorldSpace) + effect.Offset.GetLocation());
		}
		else
		{
			if(effect.AttachBoneName == "")
			{
				if (effect.N_Effect)
					UtilEffect::SpawnNiagaraEffect(GetWorld(), effect.N_Effect, Owner->GetActorLocation() + effect.Offset.GetLocation(),
						Owner->GetActorRotation() + effect.Offset.GetRotation().Rotator(), Owner->GetActorScale() + effect.Offset.GetScale3D());
				if (effect.Effect)
					UtilEffect::SpawnParticleEffect(GetWorld(), effect.Effect, Owner->GetActorLocation() + effect.Offset.GetLocation(),
						Owner->GetActorRotation() + effect.Offset.GetRotation().Rotator(), Owner->GetActorScale() + effect.Offset.GetScale3D());
				if (effect.Sound)
					UtilEffect::SpawnSoundBase(this, effect.Sound, Owner->GetActorLocation() + effect.Offset.GetLocation());
			}
			else
			{
				TArray<USkeletalMeshComponent*> comp;
				Owner->GetComponents<USkeletalMeshComponent>(comp);

				USkeletalMeshComponent* mesh = nullptr;

				for(int i = 0; i < comp.Num(); ++i)
				{
					TArray<FName> names;
					comp[i]->GetBoneNames(names);

					auto boneIndex = names.Find(effect.AttachBoneName);
					if(boneIndex != INDEX_NONE)
					{
						mesh = comp[i];
						break;
					}
				}

				if (effect.N_Effect)
					UtilEffect::SpawnNiagaraEffect(GetWorld(), effect.N_Effect, mesh->GetBoneLocation(effect.AttachBoneName, EBoneSpaces::WorldSpace),
						 mesh->GetBoneQuaternion(effect.AttachBoneName, EBoneSpaces::WorldSpace).Rotator(), Owner->GetActorScale());
				if (effect.Effect)
					UtilEffect::SpawnParticleEffect(GetWorld(), effect.Effect, mesh->GetBoneLocation(effect.AttachBoneName, EBoneSpaces::WorldSpace),
						 mesh->GetBoneQuaternion(effect.AttachBoneName, EBoneSpaces::WorldSpace).Rotator(), Owner->GetActorScale());
				if (effect.Sound)
					UtilEffect::SpawnSoundBase(this, effect.Sound, mesh->GetBoneLocation(effect.AttachBoneName, EBoneSpaces::WorldSpace));
			}
		}
	}
}

// Called every frame
void UDecoratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDecoratorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StartEffect(EEffectPlayType::Death);
}

void UDecoratorComponent::PlayWalkEffect(FName InBoneName)
{
	StartEffect(EEffectPlayType::Walk, InBoneName);
}

