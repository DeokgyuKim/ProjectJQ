// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectManagementGSS.h"
#include "../Core/ProjectJQPlayerController.h"
#include "../Manager/FstreamManager.h"

#include <Components/CapsuleComponent.h>
#include <GameFramework/Character.h>
#include <TimerManager.h>


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

	//FFstreamManager::OpenDataBinaryCustom(GetWorld()->GetMapName() + TEXT("_ObjectPoolingLog.bin"), std::ios::trunc);

	//World->GetTimerManager().SetTimer(WritePoolingObjectHandle, this, &UObjectManagementGSS::WritePoolingObjectCount, 1.f, true);
}

void UObjectManagementGSS::Deinitialize()
{
	Super::Deinitialize();

	/*
	FFstreamManager::CloseDataBinaryCustom();

	for(const TPair<FString, TMap<UClass*, FPoolingInfo>>& mapPoolingInfo : PoolingObjectInfo)
	{
		TArray<FString> outData;
		for(const TPair<UClass*, FPoolingInfo>& poolingInfo : mapPoolingInfo.Value)
		{
			outData.Add(FString::Printf(TEXT("%s:%d"), *poolingInfo.Value.UClassRefPath, poolingInfo.Value.Count / poolingInfo.Value.CheckCount));
		}
		FFstreamManager::WriteDataBinary(mapPoolingInfo.Key + TEXT("_ObjectPooling.bin"), outData);
	}
	*/
}

void UObjectManagementGSS::WritePoolingObjectCount()
{
	TMap<UClass*, int32> actorCount;
	for(TPair<int32, TWeakObjectPtr<AActor>> actor : ManagementTargets)
	{
		if(actor.Value->GetClass()->ImplementsInterface(UObjectPoolingInterface::StaticClass()))
		{
			actorCount.FindOrAdd(actor.Value->GetClass())++;
		}
	}
	
	TArray<FString> outData;
	FString data = FString::Printf(TEXT("%lf"), GetWorld()->TimeSeconds);
	outData.Add(data);
	
	for(const TPair<UClass*, int32>& count : actorCount)
	{
		data = FString::Printf(TEXT("%s:%d"), *count.Key->GetPathName(), count.Value);
		outData.Add(data);
		
		FPoolingInfo& info = PoolingObjectInfo.FindOrAdd(GetWorld()->GetMapName()).FindOrAdd(count.Key);
		info.UClassRefPath = TEXT("/Script/Engine.Blueprint'") + count.Key->GetPathName() + TEXT("'");
		info.Count += count.Value;;
		info.CheckCount++;
	}

	FFstreamManager::WriteDataBinaryCustom(outData, TEXT("|"));
}

void UObjectManagementGSS::DestroyActor(AActor* InActor)
{
	//넘겨받은 액터 포인터가 존재하는 지 확인합니다.
	int32 objectId = -1;
	if(InActor->GetClass()->ImplementsInterface(UObjectManagementTargetInterface::StaticClass()))
		objectId = Cast<IObjectManagementTargetInterface>(InActor)->GetObjectId();
	else
		return;

	TWeakObjectPtr<AActor>* destroyActor = ManagementTargets.Find(objectId);
	if(destroyActor == nullptr)
		return;
	
	if(destroyActor->IsValid())
	{
		if(InActor->GetClass()->ImplementsInterface(UObjectPoolingInterface::StaticClass()))
		{
			//PoolingObject에 추가합니다.
			Cast<IObjectPoolingInterface>(InActor)->Release();
			PoolingObject.FindOrAdd(InActor->GetClass()).Add(InActor);
			InActor->SetActorHiddenInGame(true);
		}
		else
		{
			//액터를 Detroy합니다.
			InActor->SetActorHiddenInGame(true);
			InActor->Destroy();
		}
	}
	
	//오브젝트를 관리하는 맵에서 삭제합니다.
	ManagementTargets.Remove(objectId);
}

int32 UObjectManagementGSS::AddActor(AActor* InActor)
{
	for(const TPair<int32, TWeakObjectPtr<AActor>>& target : ManagementTargets)
	{
		if(target.Value == InActor)
		{
			LOG_SCREEN(FColor::Yellow, TEXT("%s is Already Add In ManagementTargets"), *InActor->GetName())
			return INVALID_OBJECTID;
		}
	}
	
	//오브젝트ID 등록
	int32 objId = ObjectIdGenerator.GenerateID();
	SetObjectId(InActor, objId);

	ManagementTargets.Add(objId, InActor);
	return objId;
}

AActor* UObjectManagementGSS::FindActor(int32 InActorId)
{
	TWeakObjectPtr<AActor>* find = ManagementTargets.Find(InActorId);
	if(find)
		return find->Get();
	return nullptr;
}

void UObjectManagementGSS::ReAllocatePoolingObject(FString InWorldName)
{
	TArray<FString> datas;
	if(FFstreamManager::ReadDataBinary(InWorldName + TEXT("_ObjectPooling.bin"), datas))
	{
		for(int i = 1; i < datas.Num(); ++i)
		{
			FString uClassName, countStr;
			datas[i].Split(TEXT(":"), &uClassName, &countStr);
			
			FString uClassFullName = TEXT("/Script/Engine.Blueprint'") + uClassName + TEXT("'");
			UClass* uClass = LoadClass<AActor>(nullptr, *uClassFullName);

			int32 count = FCString::Atoi(*countStr);
			for(int j = 0; j < count; ++j)
			{
				AActor* actor = World->SpawnActor(uClass);
				TArray<AActor*>& actors = PoolingObject.FindOrAdd(uClass);
				actors.Add(actor);
			}
		}
	}
}
