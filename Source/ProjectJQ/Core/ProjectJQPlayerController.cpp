// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectJQPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "../Character/CharacterPC.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "JQCheatManager.h"
#include "ProjectJQ/Component/AttackComponent.h"
#include "ProjectJQ/Component/InventoryComponent.h"
#include "ProjectJQ/SubSystem/UIManagementGSS.h"
#include "ProjectJQ/UI/Inventory.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectJQPlayerController::AProjectJQPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	CheatClass = UJQCheatManager::StaticClass();
}

void AProjectJQPlayerController::BeginPlay()
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

void AProjectJQPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		JQBindAction(SetDestinationClickAction, ETriggerEvent::Started, TEXT("OnInputStarted"));
		JQBindAction(SetDestinationClickAction, ETriggerEvent::Triggered, TEXT("OnSetDestinationTriggered"));
		JQBindAction(SetDestinationClickAction, ETriggerEvent::Completed, TEXT("OnSetDestinationReleased"));
		JQBindAction(SetDestinationClickAction, ETriggerEvent::Canceled, TEXT("OnSetDestinationReleased"));
		
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProjectJQPlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnSetDestinationTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProjectJQPlayerController::OnSetDestinationReleased);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AProjectJQPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		JQBindAction(SetDestinationTouchAction, ETriggerEvent::Started, TEXT("OnInputStarted"));
		JQBindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, TEXT("OnTouchTriggered"));
		JQBindAction(SetDestinationTouchAction, ETriggerEvent::Completed, TEXT("OnTouchReleased"));
		JQBindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, TEXT("OnTouchReleased"));
		
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AProjectJQPlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnTouchTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AProjectJQPlayerController::OnTouchReleased);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AProjectJQPlayerController::OnTouchReleased);

		// Camera Zoom input events
		JQBindAction(ZoomInAction, ETriggerEvent::Triggered, TEXT("OnZoomIn"));
		JQBindAction(ZoomOutAction, ETriggerEvent::Triggered, TEXT("OnZoomOut"));
		
		//EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnZoomIn);
		//EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnZoomOut);

		JQBindAction(InventoryOnOff, ETriggerEvent::Triggered, TEXT("OnOffInventory"));
		//EnhancedInputComponent->BindAction(InventoryOnOff, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnOffInventory);

		// // Attack Input Events
		// EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::Attack);

		for(ESkillInputKey inputKeyType : TEnumRange<ESkillInputKey>())
		{
			if(inputKeyType == ESkillInputKey::None) continue;

			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Started, TEXT("SkillStarted"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Ongoing, TEXT("SkillOnGoing"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Triggered, TEXT("SkillTriggered"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Completed, TEXT("SkillCompleted"));
			JQBindAction(SkillAction[inputKeyType], ETriggerEvent::Canceled, TEXT("SkillCanceled"));
			
			//EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Started, this, &AProjectJQPlayerController::SkillStarted);
			//EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Ongoing, this, &AProjectJQPlayerController::SkillOnGoing);
			//EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Triggered, this, &AProjectJQPlayerController::SkillTriggered);
			//EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Completed, this, &AProjectJQPlayerController::SkillCompleted);
			//EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Canceled, this, &AProjectJQPlayerController::SkillCanceled);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	SaveCurrentBindAction();
}

void AProjectJQPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AProjectJQPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AProjectJQPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AProjectJQPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AProjectJQPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AProjectJQPlayerController::OnZoomIn()
{
	ACharacterPC* PC = Cast<ACharacterPC>(GetPawn());
	if(PC)
	{
		PC->GetCameraBoom()->TargetArmLength -= ZoomValue;
	}
}

void AProjectJQPlayerController::OnZoomOut()
{
	ACharacterPC* PC = Cast<ACharacterPC>(GetPawn());
	if(PC)
	{
		PC->GetCameraBoom()->TargetArmLength += ZoomValue;
	}
}

void AProjectJQPlayerController::Attack()
{
	ACharacterBase* PC = Cast<ACharacterBase>(GetPawn());
	if(PC)
	{
		UAttackComponent* AttackComponent = PC->GetComponentByClass<UAttackComponent>();
		AttackComponent->ProcessComboCommand();
	}
}

void AProjectJQPlayerController::SkillTriggered(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillTriggered(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectJQPlayerController::SkillStarted(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillStarted(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectJQPlayerController::SkillOnGoing(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillOnGoing(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectJQPlayerController::SkillCanceled(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillCanceled(GetSkillInputKeyFromAction(InSourceAction));
}

void AProjectJQPlayerController::SkillCompleted(FInputActionValue InActionValue, float InElapsedTime,
	float InTriggeredTime, const UInputAction* InSourceAction)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillCompleted(GetSkillInputKeyFromAction(InSourceAction));
}

const ESkillInputKey AProjectJQPlayerController::GetSkillInputKeyFromAction(const UInputAction* InSourceAction) const
{
	if (InSourceAction->GetName().Contains(TEXT("IA_SkillQ")))
		return ESkillInputKey::Q;
		
	else if (InSourceAction->GetName().Contains(TEXT("IA_SkillW")))
		return ESkillInputKey::W;
		
	else if (InSourceAction->GetName().Contains(TEXT("IA_SkillE")))
		return ESkillInputKey::E;
		
	else if (InSourceAction->GetName().Contains(TEXT("IA_SkillR")))
		return ESkillInputKey::R;

	else if (InSourceAction->GetName().Contains(TEXT("IA_BasicAttack")))
		return ESkillInputKey::BasicAttack;
	
	else
		return ESkillInputKey::None;
}

void AProjectJQPlayerController::OnOffInventory()
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
		
		JQUnbindAction(SetDestinationClickAction, ETriggerEvent::Started);
		JQUnbindAction(SetDestinationClickAction, ETriggerEvent::Triggered);
		JQUnbindAction(SetDestinationClickAction, ETriggerEvent::Completed);
		JQUnbindAction(SetDestinationClickAction, ETriggerEvent::Canceled);

		JQUnbindAction(SetDestinationTouchAction, ETriggerEvent::Started);
		JQUnbindAction(SetDestinationTouchAction, ETriggerEvent::Triggered);
		JQUnbindAction(SetDestinationTouchAction, ETriggerEvent::Completed);
		JQUnbindAction(SetDestinationTouchAction, ETriggerEvent::Canceled);

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

void AProjectJQPlayerController::JQBindAction(const UInputAction* InInputAction, ETriggerEvent InTriggerEvent, FName InFunctionName)
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

void AProjectJQPlayerController::JQUnbindAction(const UInputAction* InInputAction, ETriggerEvent InTriggerEvent)
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

void AProjectJQPlayerController::SaveCurrentBindAction()
{
	SavedBindActionInfos.Add(CurrentBindActionInfos);
}

void AProjectJQPlayerController::RestoreBindAction()
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
