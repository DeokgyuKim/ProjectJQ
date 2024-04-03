// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/ObjectManagementTargetInterface.h"
#include "JQProjectile.generated.h"


class UProjectileMovementComponent;
class UParticleSystem;
class UNiagaraSystem;
class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class PROJECTJQ_API AJQProjectile : public AActor, public IObjectManagementTargetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	/**
	 * 
	 */
	AJQProjectile();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JQProjectile)
	TObjectPtr<UArrowComponent> Direction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=JQProjectile)
	UProjectileMovementComponent* ProjectileComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JQProjectile)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=JQProjectile)
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=JQProjectile)
	TObjectPtr<UParticleSystemComponent> EmitterComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=JQProjectile)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=JQProjectile)
	float MaxRange;
	
	FVector StartLocation;
	FVector PrevLocation;
	
	AController* Attacker;
public:
	void Initialize(const FVector& ShootDirection, AController* Attacker);

	void SetAttacker(AController InController);
protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
};
