// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillStampComponent.h"
#include "PCSkillStampComponent.generated.h"

class ACharacterPC;
class AJQProjectile;
class UMaterialInterface;
class ADecalActor;
enum class ETriggerEvent : uint8;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTJQ_API UPCSkillStampComponent : public USkillStampComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UPCSkillStampComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	virtual void SkillStarted();
	virtual void SkillTriggered();
	virtual void SkillOnGoing();
	virtual void SkillCanceled();
	virtual void SkillCompleted();
	
protected:
	//카메라에서 마우스 커서로 레이를 쏴서 가장 처음 부딫힌 지점의 월드 좌표계를 구합니다.
	FVector GetWorldLocationAtMousePointer();
	//캐릭터에서 마우스 커서를 향하는 정규 벡터를 구합니다.
	FVector GetVector2DFromCharacterToMousePointer();
	//캐릭터를 마우스 방향으로 쳐다보게 합니다.
	void SetCharacterRotationToMousePointer(FVector InVectorToMouse) const;
};
