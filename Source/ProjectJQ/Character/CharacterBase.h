// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "../Interface/ObjectManagementTargetInterface.h"
#include "CharacterBase.generated.h"

class UDecoratorComponent;
class UStatControlComponent;

UCLASS(Blueprintable)
class ACharacterBase : public ACharacter, public IObjectManagementTargetInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatControlComponent> StatControlComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecoratorComponent> DecoratorComponent;

	ECharacterType CharacterType = ECharacterType::None;
public:
	ACharacterBase();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

public:
	ECharacterType GetCharacterType() const {return CharacterType;}

	//Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Attack, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UJQComboActionData> ComboActionData;

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer(); // 타이머 발동
	void ComboCheck();			// 타이머가 발동 되었으면 입력이 들어왔는지 체크

	int32 CurrentCombo = 0; //0 : 콤보가 시작되지 않음, n : n번째 타수 진행 중
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false; // 발동한 타이머 이전에 입력 커멘드가 들어왔는지 점검하는 필드값.

public:
	void ProcessComboCommand();
	
};

