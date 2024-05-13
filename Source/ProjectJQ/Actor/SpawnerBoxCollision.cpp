// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerBoxCollision.h"

#include <Components/BoxComponent.h>

#include "ProjectJQ/Character/CharacterPC.h"

// Sets default values
ASpawnerBoxCollision::ASpawnerBoxCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
#if WITH_EDITOR
	BoxCollision->SetHiddenInGame(false);
#endif
	RootComponent = BoxCollision;
}

// Called when the game starts or when spawned
void ASpawnerBoxCollision::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpawnerBoxCollision::BeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ASpawnerBoxCollision::EndOverlap);
}

// Called every frame
void ASpawnerBoxCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnerBoxCollision::BeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor,
	UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool InbFromSweep, const FHitResult& InSweepResult)
{
	if(ACharacterPC* pc = Cast<ACharacterPC>(InOtherActor))
	{
		Players.Add(pc);
	}
}

void ASpawnerBoxCollision::EndOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor,
	UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex)
{
	if(ACharacterPC* pc = Cast<ACharacterPC>(InOtherActor))
	{
		Players.RemoveAll([pc](TWeakObjectPtr<ACharacterPC>& player)
		{
			return !player.IsValid() || player.Get() == pc;
		});
	}
}

bool ASpawnerBoxCollision::IsPlayerInBoxComponent()
{
	return !Players.IsEmpty();
}

