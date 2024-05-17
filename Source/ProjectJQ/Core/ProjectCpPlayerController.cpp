// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCpPlayerController.h"
#include "GameFramework/Pawn.h"
#include "../Character/CharacterPC.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "JQCheatManager.h"
#include "ProjectJQ/Component/InventoryComponent.h"
#include "ProjectJQ/SubSystem/UIManagementGSS.h"
#include "ProjectJQ/UI/Inventory.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectCpPlayerController::AProjectCpPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	CheatClass = UJQCheatManager::StaticClass();
}

void AProjectCpPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	UUIManagementGSS* gss = GetGameInstance()->GetSubsystem<UUIManagementGSS>();
	if(gss)
	{
		gss->SetController(this);
	}
}

void AProjectCpPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RotateTowardsPickedPoint();
}

void AProjectCpPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		JQBindAction(MoveAction, ETriggerEvent::Triggered, TEXT("Move"));

		// Camera Zoom input events
		JQBindAction(ZoomInAction, ETriggerEvent::Triggered, TEXT("OnZoomIn"));
		JQBindAction(ZoomOutAction, ETriggerEvent::Triggered, TEXT("OnZoomOut"));

		JQBindAction(InventoryOnOff, ETriggerEvent::Triggered, TEXT("OnOffInventory"));

		for(ESkillInputKey inputKeyType : TEnumRange<ESkillInputKey>())
		{
			if(inputKeyType == ESkillInputKey::None) continue;

			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Started, TEXT("SkillStarted"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Ongoing, TEXT("SkillOnGoing"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Triggered, TEXT("SkillTriggered"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Completed, TEXT("SkillCompleted"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Canceled, TEXT("SkillCanceled"));
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	SaveCurrentBindAction();
}

void AProjectCpPlayerController::Move(FInputActionValue InActionValue, float InElapsedTime, float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* PC = Cast<ACharacterPC>(GetPawn());
	if(PC)
	{
		PC->Move(InActionValue);
	}
}

void AProjectCpPlayerController::OnZoomIn()
{
	ACharacterPC* PC = Cast<ACharacterPC>(GetPawn());
	if(PC)
	{
		PC->GetCameraBoom()->TargetArmLength -= ZoomValue;
	}
}

void AProjectCpPlayerController::OnZoomOut()
{
	ACharacterPC* PC = Cast<ACharacterPC>(GetPawn());
	if(PC)
	{
		PC->GetCameraBoom()->TargetArmLength += ZoomValue;
	}
}

void AProjectCpPlayerController::SkillTriggered(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillTriggered(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectCpPlayerController::SkillStarted(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillStarted(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectCpPlayerController::SkillOnGoing(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillOnGoing(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectCpPlayerController::SkillCanceled(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillCanceled(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectCpPlayerController::SkillCompleted(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillCompleted(GetSkillInputKeyFromAction(InSourceAction));
}

const ESkillInputKey AProjectCpPlayerController::GetSkillInputKeyFromAction(const UInputAction* InSourceAction) const
{
	if (InSourceAction->GetName().Contains(TEXT("IA_SkillQ")))
		return ESkillInputKey::Q;
		
	else if (InSourceAction->GetName().Contains(TEXT("IA_SkillE")))
		return ESkillInputKey::E;
		
	else if (InSourceAction->GetName().Contains(TEXT("IA_SkillR")))
		return ESkillInputKey::R;

	else if (InSourceAction->GetName().Contains(TEXT("IA_BasicAttack")))
		return ESkillInputKey::BasicAttack;
	
	else
		return ESkillInputKey::None;
}

void AProjectCpPlayerController::OnOffInventory()
{
	UInventoryComponent* comp = Cast<UInventoryComponent>(GetPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));
	if(comp == nullptr)
		return;

	UInventory* inven = comp->GetInventoryUI();
	//인벤토리 UI Off
	if(inven->GetVisibility() == ESlateVisibility::Visible)
	{
		inven->SetVisibility(ESlateVisibility::Collapsed);
		inven->RemoveFromParent();
		RestoreBindAction();
	}
	//인벤토리 UI On
	else
	{
		inven->SetVisibility(ESlateVisibility::Visible);
		inven->AddToViewport();

		JQUnbindAction(MoveAction, ETriggerEvent::Triggered);

		JQUnbindAction(ZoomInAction, ETriggerEvent::Triggered);
		JQUnbindAction(ZoomOutAction, ETriggerEvent::Triggered);

		for(ESkillInputKey inputKeyType : TEnumRange<ESkillInputKey>())
		{
			if(inputKeyType == ESkillInputKey::None) continue;

			JQUnbindAction(SkillAction[inputKeyType], ETriggerEvent::Started);
			JQUnbindAction(SkillAction[inputKeyType], ETriggerEvent::Ongoing);
			JQUnbindAction(SkillAction[inputKeyType], ETriggerEvent::Triggered);
			JQUnbindAction(SkillAction[inputKeyType], ETriggerEvent::Completed);
			JQUnbindAction(SkillAction[inputKeyType], ETriggerEvent::Canceled);
		}
	}
}

void AProjectCpPlayerController::JQBindAction(const UInputAction* InInputAction, ETriggerEvent InTriggerEvent, FName InFunctionName)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		BindActionInfo info;
		info.IA = InInputAction;
		info.TriggerEvent = InTriggerEvent;
		info.FunctionName = InFunctionName;
		info.Handle = EnhancedInputComponent->BindAction(InInputAction, InTriggerEvent, this, InFunctionName).GetHandle();
		CurrentBindActionInfos.Add(info);
	}
}

void AProjectCpPlayerController::JQUnbindAction(const UInputAction* InInputAction, ETriggerEvent InTriggerEvent)
{
	for(TArray<BindActionInfo>::TIterator iter(CurrentBindActionInfos.CreateIterator()); iter; ++iter)
	{
		if(iter->IA == InInputAction && iter->TriggerEvent == InTriggerEvent)
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
			{
				if(!EnhancedInputComponent->RemoveBindingByHandle(iter->Handle))
					EnhancedInputComponent->RemoveActionBindingForHandle(iter->Handle);
			}
			
			iter.RemoveCurrent();
		}
	}
}

void AProjectCpPlayerController::SaveCurrentBindAction()
{
	SavedBindActionInfos.Add(CurrentBindActionInfos);
}

void AProjectCpPlayerController::RestoreBindAction()
{
	if(SavedBindActionInfos.IsEmpty())
		return;

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//기존 바인딩 해제
		EnhancedInputComponent->ClearActionBindings();
		CurrentBindActionInfos.Empty();

		//저장된 바인딩액션 인포 중 가장 위의 데이터 복구
		for(const BindActionInfo& info : SavedBindActionInfos[SavedBindActionInfos.Num() - 1])
		{
			JQBindAction(info.IA, info.TriggerEvent, info.FunctionName);
		}
	}
	//루트 바인딩 액션 인포가 아니면 Pop
	if(SavedBindActionInfos.Num() != 1)
	{
		SavedBindActionInfos.Pop();
	}
}

void AProjectCpPlayerController::RotateTowardsPickedPoint()
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	FHitResult hit;
	
	GetHitResultUnderCursor(ECC_Visibility, false, hit);

	FVector underCursor = hit.Location;
	underCursor.Z = 0.0;

	FVector playerPosition = pc->GetActorLocation();
	playerPosition.Z = 0.0f;

	FRotator rotation = (underCursor - playerPosition).Rotation();

	pc->SetActorRotation(rotation);
}
