// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerBoxCollision.generated.h"

class ACharacterPC;
class AMonsterSpawner;
class UBoxComponent;

UCLASS()
class PROJECTJQ_API ASpawnerBoxCollision : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UBoxComponent* BoxCollision = nullptr;

	TArray<TWeakObjectPtr<ACharacterPC>> Players;
public:	
	// Sets default values for this actor's properties
	ASpawnerBoxCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp,
		int32 InOtherBodyIndex, bool InbFromSweep, const FHitResult& InSweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex);

	bool IsPlayerInBoxComponent();
};
