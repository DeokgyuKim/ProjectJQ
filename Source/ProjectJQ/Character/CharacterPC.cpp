// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterPC.h"

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
#include "Engine/World.h"

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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomInRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_ZoomIn.IA_ZoomIn'"));
	if(InputActionZoomInRef.Object)
	{
		ZoomInAction = InputActionZoomInRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomOutRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_ZoomOut.IA_ZoomOut'"));
	if(InputActionZoomOutRef.Object)
	{
		ZoomOutAction = InputActionZoomOutRef.Object;
	}

	CharacterType = ECharacterType::Player;

	ZoomValue = 20.0f;
}

void ACharacterPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ACharacterPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &ACharacterPC::CameraZoomIn);
	EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &ACharacterPC::CameraZoomOut);
}

void ACharacterPC::CameraZoomIn(const FInputActionValue& Value)
{
	CameraBoom->TargetArmLength -= ZoomValue;
	//UE_LOG(LogTemp, Log, TEXT("ZOOMIN"));
}

void ACharacterPC::CameraZoomOut(const FInputActionValue& Value)
{
	CameraBoom->TargetArmLength += ZoomValue;
	//UE_LOG(LogTemp, Log, TEXT("ZOOMOUT"));
}