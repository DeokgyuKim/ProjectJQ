// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectJQ.h"
#include "Modules/ModuleManager.h"
#include <Kismet/GameplayStatics.h>

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ProjectJQ, "ProjectJQ" );

DEFINE_LOG_CATEGORY(LogProjectJQ)

UWorld* JQGetWorld(UObject* InObject)
{
	if (InObject)
		return InObject->GetWorld();

	FWorldContext* world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
	if (!world )
		return nullptr;
	
	return world->World();
}

UGameInstance* JQGetGameInstance(UWorld* InWorld)
{
	UWorld* world = InWorld ? InWorld : JQGetWorld();

	return UGameplayStatics::GetGameInstance( world );
}
