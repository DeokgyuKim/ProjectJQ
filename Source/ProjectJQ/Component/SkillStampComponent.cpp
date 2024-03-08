// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStampComponent.h"

// Sets default values for this component's properties
USkillStampComponent::USkillStampComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USkillStampComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USkillStampComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillStampComponent::SkillTriggered()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillTriggerd"), *GetName());
}

void USkillStampComponent::SkillStarted()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillStarted"), *GetName());
}

void USkillStampComponent::SkillOnGoing()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillOnGoing"), *GetName());
}

void USkillStampComponent::SkillCanceled()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillCanceled"), *GetName());
}

void USkillStampComponent::SkillCompleted()
{
	LOG_SCREEN(FColor::White, TEXT("%s: SkillCompleted"), *GetName());
}

