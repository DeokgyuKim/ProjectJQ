// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectManagementGSS.h"

#include <Components/CapsuleComponent.h>
#include <GameFramework/Character.h>

#include "ProjectJQ/Core/ProjectJQPlayerController.h"

void UObjectManagementGSS::SetObjectId(AActor* InActor, int32 InObjectId)
{
	if(InActor->GetClass()->ImplementsInterface(UObjectManagementTargetInterface::StaticClass()))
	{
		IObjectManagementTargetInterface* interfaceManagement = Cast<IObjectManagementTargetInterface>(InActor);
		interfaceManagement->SetObjectId(InObjectId);
	}
}

FVector UObjectManagementGSS::GetLocationByCharacter(AActor* InActor, const FVector& InLocation)
{
	ACharacter* character = Cast<ACharacter>(InActor);

	FVector outLocation = InLocation;
	if(character)
		outLocation.Z += character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	return outLocation;
}

void UObjectManagementGSS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ObjectIdGenerator.Initialize(0, 10000);

	World = GetWorld();
	
	//Player Controller를 찾습니다.
	for(FConstPlayerControllerIterator iter = World->GetPlayerControllerIterator(); iter; ++iter)
	{
		if(iter->IsValid() && Cast<AProjectJQPlayerController>(iter->Get()) != nullptr)
		{
			Controller = Cast<AProjectJQPlayerController>(iter->Get());
			break;
		}
	}
}

void UObjectManagementGSS::Deinitialize()
{
	Super::Deinitialize();
}
