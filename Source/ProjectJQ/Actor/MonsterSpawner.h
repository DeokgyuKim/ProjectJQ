// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

class ACharacterMonster;
class ACharacterBase;
class UBoxComponent;

USTRUCT()
struct FToolActorInfo
{
	GENERATED_BODY()

	FString CharacterBPName = TEXT("");
	FTransform Transform = FTransform::Identity;
};
USTRUCT()
struct FSpawnerLayer
{
	GENERATED_BODY()
	
	TWeakObjectPtr<AMonsterSpawner> Owner;

#if WITH_EDITORONLY_DATA
	//툴 캐릭터
	UPROPERTY(VisibleInstanceOnly)
	TArray<TWeakObjectPtr<ACharacterBase>> ToolCharacters;
#endif

	//해당 레이어 몬스터 스폰조건
	UPROPERTY(EditInstanceOnly)
	TArray<EMonsterSpawnerLayerConditionType> LayerConditionTypes;

	TArray<FToolActorInfo> ActorInfos;
};

UCLASS()
class PROJECTJQ_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent = nullptr;
	
	UPROPERTY(EditInstanceOnly, Category="JQ_Tool")
	int32 LayerIndex = 0;
	
	UPROPERTY(EditInstanceOnly, Category="JQ_Tool")
	FString SpawnMonsterBpName = TEXT("");
	
	UFUNCTION(CallInEditor, Category="JQ_Tool")
	void SpawnMonster();
#endif
	
	UPROPERTY(EditInstanceOnly, Category="JQ_Tool")
	double Range = 1000.0;

	UPROPERTY(EditInstanceOnly, Category="JQ_Tool")
	TArray<FSpawnerLayer> SpawnLayers;

	bool IsOverlapPlayer = false;

	TArray<TWeakObjectPtr<ACharacterMonster>> SpawnMonsters;

public:	
	// Sets default values for this actor's properties
	AMonsterSpawner();

	// Called change info in editor
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Layer Condition//////////////////////////////////////////////////////
	//레이어 범위 내에 플레이어가 있는 지 판별
	bool LayerConditionPlayerInRange();
	//this몬스터 스포너가 스폰한 몬스터가 모두 죽었는 지 판별
	bool LayerConditionPreLayerMonsterAllDead();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
