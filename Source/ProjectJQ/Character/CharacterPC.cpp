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
#include "ProjectJQ/Component/SkillStampComponent.h"

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
		USkillStampComponent* skillStampComp = NewObject<USkillStampComponent>(this, skillStamp.Value);
		skillStampComp->RegisterComponent();
		if(skillStampComp->HasBeenInitialized() == false)
			skillStampComp->InitializeComponent();
		Skills.Add(skillStamp.Key, skillStampComp);
		
	}
}

void ACharacterPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ACharacterPC::SkillTriggered(ESkillInputKey InInputKey)
{
	TObjectPtr<USkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillTriggered();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillStarted(ESkillInputKey InInputKey)
{
	TObjectPtr<USkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillStarted();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillOnGoing(ESkillInputKey InInputKey)
{
	TObjectPtr<USkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillOnGoing();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillCanceled(ESkillInputKey InInputKey)
{
	TObjectPtr<USkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillCanceled();
	CurrentSkill = *findSkill;
}

void ACharacterPC::SkillCompleted(ESkillInputKey InInputKey)
{
	TObjectPtr<USkillStampComponent>* findSkill = Skills.Find(InInputKey);
	if(findSkill == nullptr || *findSkill == nullptr)
		return;

	(*findSkill)->SkillCompleted();
	CurrentSkill = *findSkill;
}
