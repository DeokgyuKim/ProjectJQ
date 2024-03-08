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
}

void AProjectJQPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProjectJQPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProjectJQPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AProjectJQPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AProjectJQPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AProjectJQPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AProjectJQPlayerController::OnTouchReleased);

		// Camera Zoom input events
		EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnZoomIn);
		EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::OnZoomOut);

		// Attack Input Events
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AProjectJQPlayerController::Attack);

		for(ESkillInputKey inputKeyType : TEnumRange<ESkillInputKey>())
		{
			EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Started, this, &AProjectJQPlayerController::SkillStarted);
			EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Ongoing, this, &AProjectJQPlayerController::SkillOnGoing);
			EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Triggered, this, &AProjectJQPlayerController::SkillTriggered);
			EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Completed, this, &AProjectJQPlayerController::SkillCompleted);
			EnhancedInputComponent->BindAction(SkillAction[inputKeyType], ETriggerEvent::Canceled, this, &AProjectJQPlayerController::SkillCanceled);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
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

void AProjectJQPlayerController::SkillTriggered(const FInputActionInstance& InInstance)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillTriggered(GetSkillInputKeyFromAction(InInstance));
}

void AProjectJQPlayerController::SkillStarted(const FInputActionInstance& InInstance)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillStarted(GetSkillInputKeyFromAction(InInstance));
}

void AProjectJQPlayerController::SkillOnGoing(const FInputActionInstance& InInstance)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillOnGoing(GetSkillInputKeyFromAction(InInstance));
}

void AProjectJQPlayerController::SkillCanceled(const FInputActionInstance& InInstance)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillCanceled(GetSkillInputKeyFromAction(InInstance));
}

void AProjectJQPlayerController::SkillCompleted(const FInputActionInstance& InInstance)
{
	ACharacterPC* pc = Cast<ACharacterPC>(GetCharacter());
	if(pc == nullptr)
		return;
	
	pc->SkillCompleted(GetSkillInputKeyFromAction(InInstance));
}

const ESkillInputKey AProjectJQPlayerController::GetSkillInputKeyFromAction(const FInputActionInstance &inInstance) const
{
	if (const UInputAction *inputAction = inInstance.GetSourceAction())
	{
		if (inputAction->GetName().Contains(TEXT("IA_SkillQ")))
			return ESkillInputKey::Q;
		
		else if (inputAction->GetName().Contains(TEXT("IA_SkillW")))
			return ESkillInputKey::W;
		
		else if (inputAction->GetName().Contains(TEXT("IA_SkillE")))
			return ESkillInputKey::E;
		
		else if (inputAction->GetName().Contains(TEXT("IA_SkillR")))
			return ESkillInputKey::R;
	}
	return ESkillInputKey::None;
}