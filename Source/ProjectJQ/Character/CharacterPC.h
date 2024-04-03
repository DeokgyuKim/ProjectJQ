// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "CharacterBase.h"
#include "InputActionValue.h"
#include "CharacterPC.generated.h"

class USkillStampComponent;

UCLASS(Blueprintable)
class ACharacterPC : public ACharacterBase
{
	GENERATED_BODY()

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, Category="JQ_Skill")
	TMap<ESkillInputKey, TSubclassOf<USkillStampComponent>> OwnSkills;

	UPROPERTY()
	TWeakObjectPtr<USkillStampComponent> CurrentSkill = nullptr;

	UPROPERTY()
	TMap<ESkillInputKey, TObjectPtr<USkillStampComponent>> Skills;

public:
	ACharacterPC();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

public:
	void SkillTriggered(ESkillInputKey InInputKey);
    void SkillStarted(ESkillInputKey InInputKey);
    void SkillOnGoing(ESkillInputKey InInputKey);
    void SkillCanceled(ESkillInputKey InInputKey);
    void SkillCompleted(ESkillInputKey InInputKey);
	
public:
	UFUNCTION(BlueprintCallable)
	USkillStampComponent* GetCurrentSkill(){return CurrentSkill.Get();}
};

