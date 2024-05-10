// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "ProjectJQ/Character/CharacterBase.h"
#include "ProjectJQ/Character/CharacterMonster.h"
#include "ProjectJQ/Character/CharacterPC.h"
#include "ProjectJQ/SubSystem/ObjectManagementGSS.h"
#include "ProjectJQ/SubSystem/ReferencePathGSS.h"

void AMonsterSpawner::SpawnMonster()
{
	if(SpawnLayers.Num() <= LayerIndex)
		return;

	FString referencePath = FString::Printf(TEXT("/Script/Engine.Blueprint'/Game/ProjectJQ/Blueprints/Character/%s.%s_C'"), *SpawnMonsterBpName, *SpawnMonsterBpName);

	UClass* uClass = LoadClass<ACharacterMonster>(nullptr, *referencePath);
	if(uClass)
	{
		ACharacterMonster* monster = GetWorld()->SpawnActor<ACharacterMonster>(uClass, GetActorLocation(), GetActorRotation());
		SpawnLayers[LayerIndex].ToolCharacters.Add(monster);
	}
}

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITOR
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"));
	if(meshAsset.Succeeded())
		MeshComponent->SetStaticMesh(meshAsset.Object);
	
	RootComponent = MeshComponent;
#endif
	
	
}

void AMonsterSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	for(FSpawnerLayer& layer : SpawnLayers)
	{
		layer.Owner = this;
		layer.ToolCharacters.RemoveAll([](TWeakObjectPtr<ACharacterBase> tool)
		{
			return !tool.IsValid();
		});
	}
}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	Algo::Reverse(SpawnLayers);

	for(FSpawnerLayer& layer : SpawnLayers)
	{
		for(TWeakObjectPtr<ACharacterBase>& toolChar : layer.ToolCharacters)
		{
			FToolActorInfo info;
			info.Transform = toolChar->GetActorTransform();
			info.CharacterBPName = toolChar->GetClass()->GetName();
			info.CharacterBPName.RemoveAt(info.CharacterBPName.Len() - 2, 2);
			layer.ActorInfos.Add(info);
			toolChar->SetActorHiddenInGame(true);
			toolChar->Destroy();
		}
		layer.ToolCharacters.Empty();
	}

}

bool AMonsterSpawner::LayerConditionPlayerInRange()
{
	if(UObjectManagementGSS* gss = GetGameInstance()->GetSubsystem<UObjectManagementGSS>())
	{
		TArray<ACharacterPC*> pcs = gss->GetPlayers();
		for(const ACharacterPC* pc : pcs)
		{
			if(FVector::Distance(pc->GetActorLocation(), GetActorLocation()) <= Range)
				return true;
		}
	}
	
	return false;
}

bool AMonsterSpawner::LayerConditionPreLayerMonsterAllDead()
{
	return SpawnMonsters.IsEmpty();
}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnMonsters.RemoveAll([](TWeakObjectPtr<ACharacterMonster> InMonster)
	{
		return !InMonster.IsValid();
	});

	if(!SpawnLayers.IsEmpty())
	{
		FSpawnerLayer layer = SpawnLayers.Top();
		bool condition = true;
		//몬스터 스포너의 레이어가 스폰 조건에 맞는 지 확인
		for(const EMonsterSpawnerLayerConditionType& LayerConditionType : layer.LayerConditionTypes)
		{
			switch (LayerConditionType)
			{
			case EMonsterSpawnerLayerConditionType::PlayerInRange:
				condition = condition && LayerConditionPlayerInRange();
				break;
			case EMonsterSpawnerLayerConditionType::PreLayerMonsterAllDead:
				condition = condition && LayerConditionPreLayerMonsterAllDead();
				break;
			default:
				LOG_SCREEN(FColor::Red, TEXT("MonsterSpawner : %s 의 LayerCondition이 잘못 설정되었습니다."), *GetName());
				break;
			}
		}

		//스폰 조건에 해당한다면 레이어에 등록된 몬스터 스폰
		if(condition)
		{
			for(const FToolActorInfo& info : layer.ActorInfos)
			{
				UReferencePathGSS* uiGss = GetGameInstance()->GetSubsystem<UReferencePathGSS>();
				if(!uiGss)
					return;

				FString referencePath = uiGss->GetReferencePath(info.CharacterBPName);
				if(referencePath == TEXT(""))
					return;

				UObjectManagementGSS* omGss = GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
				if(!omGss)
					return;

				FSpawnParam spawnParam;
				spawnParam.Location = info.Transform.GetLocation();
				spawnParam.Rotation = info.Transform.GetRotation().Rotator();
				spawnParam.CallbackSpawn = nullptr;
				ACharacterMonster* monster = omGss->CreateActor<ACharacterMonster>(LoadClass<ACharacterMonster>(nullptr, *referencePath), spawnParam);
				SpawnMonsters.Add(monster);
			}

			//몬스터 스폰후 생성된 레이어 삭제
			SpawnLayers.Pop();
		}
	}

}