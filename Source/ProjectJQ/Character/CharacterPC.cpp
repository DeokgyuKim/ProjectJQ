// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterPC.h"

#include "Editor.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "../SubSystem/ObjectManagementGSS.h"
#include "ProjectJQ/Component/PCSkillStampComponent.h"

ACharacterPC::ACharacterPC()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CharacterType = ECharacterType::Player;
}

void ACharacterPC::BeginPlay()
{
	Super::BeginPlay();
	
	UObjectManagementGSS* subsystem = GetGameInstance()->GetSubsystem<UObjectManagementGSS>();
	if(subsystem != nullptr)
		subsystem->AddActor(this);

	for(const TPair<ESkillInputKey, TSubclassOf<USkillStampComponent>>& skillStamp : OwnSkills)
	{
		UPCSkillStampComponent* skillStampComp = NewObject<UPCSkillStampComponent>(this, skillStamp.Value);
		skillStampComp->RegisterComponent();
		if(skillStampComp->HasBeenInitialized() == false)
			skillStampComp->InitializeComponent();
		Skills.Add(skillStamp.Key, skillStampComp);
		
	}
}

void ACharacterPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	RotateTowardsPickedPoint();
}

void ACharacterPC::Move(const FInputActionValue& InValue)
{
	// input is a Vector2D
	FVector2D MovementVector = InValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACharacterPC::SkillTriggered(ESkillInputKey InInputKey)
{
	TObjectPtr<UPCSkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillTriggered();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillStarted(ESkillInputKey InInputKey)
{
	TObjectPtr<UPCSkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillStarted();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillOnGoing(ESkillInputKey InInputKey)
{
	TObjectPtr<UPCSkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillOnGoing();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillCanceled(ESkillInputKey InInputKey)
{
	TObjectPtr<UPCSkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillCanceled();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillCompleted(ESkillInputKey InInputKey)
{
	TObjectPtr<UPCSkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillCompleted();
	CurrentSkill = *findSkill;
}

void ACharacterPC::RotateTowardsPickedPoint()
{
	FHitResult hit;
	if(APlayerController* controller = Cast<APlayerController>(GetController()))
	{
		controller->GetHitResultUnderCursor(ECC_Visibility, false, hit);

		FVector underCursor = hit.Location;
		underCursor.Z = 0.0;

		FVector playerPosition = GetActorLocation();
		playerPosition.Z = 0.0f;

		FRotator rotation = (underCursor - playerPosition).Rotation();

		SetActorRotation(rotation);
	}
}
