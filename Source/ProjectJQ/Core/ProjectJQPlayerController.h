// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Templates/SubclassOf.h>
#include <GameFramework/PlayerController.h>

#include "InputAction.h"
#include "ProjectJQPlayerController.generated.h"

enum class ETriggerEvent : uint8;
class UInputAction;

struct BindActionInfo
{
	const UInputAction* IA;
	ETriggerEvent TriggerEvent;
	uint32 Handle;
	FName FunctionName;
};

#define BindActionFunctionName(FunctionName) &AProjectJQPlayerController::##FunctionName

struct FInputActionInstance;
/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AProjectJQPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectJQPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta = (AllowPrivateAccess = "true"))
	UInputAction *MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ZoomInAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ZoomOutAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InventoryOnOff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TMap<ESkillInputKey, UInputAction*> SkillAction;

	TArray<BindActionInfo> CurrentBindActionInfos;

	TArray<TArray<BindActionInfo>> SavedBindActionInfos;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	UFUNCTION()
	void Move(FInputActionValue InActionValue, float InElapsedTime, float InTriggeredTime, const UInputAction* InSourceAction);

	/* Camera Zoom Action */
	UFUNCTION()
	void OnZoomIn();
	UFUNCTION()
	void OnZoomOut();
	
	/* Skill Action*/
	UFUNCTION()
	void SkillTriggered(FInputActionValue InActionValue, float InElapsedTime, float InTriggeredTime, const UInputAction* InSourceAction);
	UFUNCTION()
	void SkillStarted(FInputActionValue InActionValue, float InElapsedTime, float InTriggeredTime, const UInputAction* InSourceAction);
	UFUNCTION()
	void SkillOnGoing(FInputActionValue InActionValue, float InElapsedTime, float InTriggeredTime, const UInputAction* InSourceAction);
	UFUNCTION()
	void SkillCanceled(FInputActionValue InActionValue, float InElapsedTime, float InTriggeredTime, const UInputAction* InSourceAction);
	UFUNCTION()
	void SkillCompleted(FInputActionValue InActionValue, float InElapsedTime, float InTriggeredTime, const UInputAction* InSourceAction);

	const ESkillInputKey GetSkillInputKeyFromAction(const UInputAction* InSourceAction) const;

	UFUNCTION()
	void OnOffInventory();

	float ZoomValue = 20.0f;

	void JQBindAction(const UInputAction* InInputAction, ETriggerEvent InTriggerEvent, FName InFunctionName);
	void JQUnbindAction(const UInputAction* InInputAction, ETriggerEvent InTriggerEvent);

	void SaveCurrentBindAction();

	void RestoreBindAction();

private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	
};

