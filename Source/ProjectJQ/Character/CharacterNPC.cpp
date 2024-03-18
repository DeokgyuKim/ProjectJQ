// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterNPC.h"

#include "Components/CapsuleComponent.h"
#include "../Component/JQWidgetComponent.h"

ACharacterNPC::ACharacterNPC()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	WidgetComponent = CreateDefaultSubobject<UJQWidgetComponent>(TEXT("HpBar"));
	WidgetComponent->SetDrawSize(FVector2D(50.0, 10.0));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetupAttachment(GetCapsuleComponent());
}

void ACharacterNPC::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterNPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
