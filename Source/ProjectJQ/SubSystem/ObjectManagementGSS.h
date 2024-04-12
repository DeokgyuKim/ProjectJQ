// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>
#include "../Interface/ObjectManagementTargetInterface.h"
#include "../Interface/ObjectPoolingInterface.h"

#include "ObjectManagementGSS.generated.h"

class AProjectJQPlayerController;
class AProjectJQCharacter;

struct FSpawnParam
{
	FRotator Rotation;
	FVector Location;
	std::function<void(AActor*)> CallbackSpawn;
};

struct FPoolingInfo
{
	FString UClassRefPath;
	int32	Count;
	int32	CheckCount;
};

/**
 * 오브젝트를 관리하는 GameInstanceSubSystem입니다. ObjectManagementTargetInterface를 상속받은 액터들을 관리합니다.
 * ObjectManagementTargetInterface를 상속받은 액터를 생성, 보관, 검색, 삭제의 기능을 제공합니다. 
 */
UCLASS()
class PROJECTJQ_API UObjectManagementGSS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	TArray<TWeakObjectPtr<AActor>>				ManagementTargets;
	FIdGenerator<int32>							ObjectIdGenerator;
	TWeakObjectPtr<UWorld>						World;
	TWeakObjectPtr<AProjectJQPlayerController>	Controller;

	TMap<UClass*, TArray<AActor*>>				PoolingObject;
	TMap<FString, TMap<UClass*, FPoolingInfo>>	PoolingObjectInfo;

	FTimerHandle								WritePoolingObjectHandle;
public:
	static constexpr int32 INVALID_OBJECTID = -9999;
private:
	//오브젝트 ID를 세팅합니다.
	void SetObjectId(AActor* InActor, int32 InObjectId);

	//캐릭터일 경우에 캡슐컴포넌트의 Half Height만큼 위로 올린 값을 반환합니다.
	FVector GetLocationByCharacter(AActor* InActor, const FVector& InLocation);
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
public:
	void WritePoolingObjectCount();
public:
	//액터를 생성합니다. ObjectManagementTargetInterface를 상속받지 않은 클래스는 생성하지 않습니다.
	template<typename T>
	T* CreateActor(UClass* InClass, const FSpawnParam& InSpawnParam, AActor* InOwner = nullptr);

	//액터의 포인터를 받아 액터를 파괴합니다.
	void DestroyActor(AActor* InActor);

	int32 AddActor(AActor* InActor);

	//InPredicate에 부합하는 액터를 반환합니다. 가장 먼저 부합하는 액터를 부합합니다.
	template<typename T>
	T* FindActorByPredicate(const std::function<bool(AActor*)>& InPredicate);

	template<typename T>
	void FindActorArrayByPredicate(TArray<TWeakObjectPtr<T>>& OutActors, const std::function<bool(AActor*)>& InPredicate);

	void ReAllocatePoolingObject(FString InWorldName);
};

template <typename T>
T* UObjectManagementGSS::CreateActor(UClass* InClass, const FSpawnParam& InSpawnParam, AActor* InOwner)
{
	if(!InClass)
		return nullptr;

	if(!InClass->ImplementsInterface(UObjectManagementTargetInterface::StaticClass()))
		return nullptr;

	//현재 월드 업데이트
	World = GetWorld();

	//트랜스폼 생성
	FTransform spawnTransform(InSpawnParam.Rotation, InSpawnParam.Location);


	AActor* actor = nullptr;
	if(InClass->ImplementsInterface(UObjectPoolingInterface::StaticClass()) && PoolingObject.Find(InClass) != nullptr)
	{
		actor = *PoolingObject.Find(InClass)->CreateIterator();
		PoolingObject.Find(InClass)->CreateIterator().RemoveCurrent();

		if(actor == nullptr)
			return nullptr;
		
		actor->SetActorLocation(InSpawnParam.Location);
		actor->SetActorRotation(InSpawnParam.Rotation);
		InSpawnParam.CallbackSpawn(actor);

		//오브젝트ID 등록
		int32 objId = ObjectIdGenerator.GenerateID();
		SetObjectId(actor, objId);

		Cast<IObjectPoolingInterface>(actor)->Initialize();

		actor->DispatchBeginPlay();
	}
	else
	{
		//액터 생성
		actor = World->SpawnActorDeferred<T>(InClass, spawnTransform, InOwner, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if(!actor)
			return nullptr;

		//캐릭터일 경우에 캡슐 절반높이만큼 위로 올려스폰
		FVector location = GetLocationByCharacter(actor, InSpawnParam.Location);
		spawnTransform = FTransform(InSpawnParam.Rotation, location);

		//콜백함수 호출
		if(InSpawnParam.CallbackSpawn)
			InSpawnParam.CallbackSpawn(actor);
		
		//오브젝트ID 등록
		int32 objId = ObjectIdGenerator.GenerateID();
		SetObjectId(actor, objId);
		
		//스폰 마무리
		actor->FinishSpawning(spawnTransform);
	}
	
	ManagementTargets.Add(actor);

	return Cast<T>(actor);
}

template <typename T>
T* UObjectManagementGSS::FindActorByPredicate(const std::function<bool(AActor*)>& InPredicate)
{
	for(const TWeakObjectPtr<AActor>& target : ManagementTargets)
	{
		T* castTarget = Cast<T>(target);
		
		if(castTarget != nullptr && InPredicate(target.Get()))
			return castTarget;
	}

	return nullptr;
}

template <typename T>
void UObjectManagementGSS::FindActorArrayByPredicate(TArray<TWeakObjectPtr<T>>& OutActors,
	const std::function<bool(AActor*)>& InPredicate)
{
	for(const TWeakObjectPtr<AActor>& target : ManagementTargets)
	{
		T* castTarget = Cast<T>(target);
		
		if(castTarget != nullptr && InPredicate(target.Get()))
			OutActors.Add(castTarget);
	}
}
