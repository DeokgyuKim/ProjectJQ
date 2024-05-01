// Fill out your copyright notice in the Description page of Project Settings.


#include "JQProjectile.h"
#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectJQ/Character/CharacterMonster.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"

// Sets default values
AJQProjectile::AJQProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	EmitterComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EmitterComponent"));
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	
	RootComponent = Direction;
	StaticMeshComponent->SetRelativeRotation(FRotator(-90, 0, 0));
	
	EmitterComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetupAttachment(RootComponent);
	CollisionBox->SetupAttachment(RootComponent);
	ProjectileComponent->SetUpdatedComponent(RootComponent);
	StaticMeshComponent->SetupAttachment(RootComponent);
	
	
	
	ProjectileComponent->InitialSpeed = 500.f;
	ProjectileComponent->MaxSpeed = 500.f;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->ProjectileGravityScale = 0.f;

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AJQProjectile::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void AJQProjectile::BeginPlay()
{
	Super::BeginPlay();
	EmitterComponent->Activate();
	NiagaraComponent->Activate();
}

// Called every frame
void AJQProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (FVector::Dist(StartLocation, GetActorLocation()) >= MaxRange)
	{
		UObjectManagementGSS* gss = GetOwner()->GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
		gss->DestroyActor(this);
		//Destroy();
	}

	PrevLocation = GetActorLocation();
}

void AJQProjectile::Initialize(const FVector& ShootDirection, AController* InAttacker, float InMaxRange)
{
	StartLocation = PrevLocation = GetActorLocation();
	ProjectileComponent->Velocity = ShootDirection * ProjectileComponent->InitialSpeed;
	MaxRange = InMaxRange;
	Attacker = InAttacker;
}

void AJQProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterMonster* monster = Cast<ACharacterMonster>(OtherActor);
	if(monster)
	{
		FDamageEvent DamageEvent;
		LOG_SCREEN(FColor::White, TEXT("%s Takes %d Damage"), *monster->GetName(), 100);
		monster->TakeDamage(100.f, DamageEvent, Attacker, this);
	}
}


