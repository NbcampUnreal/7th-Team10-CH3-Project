#include "02_Characters/BTPS_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"


ABTPS_PlayerCharacter::ABTPS_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 800.f;
	SprintSpeedMultiplier = 1.8f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

// void ABTPS_PlayerCharacter::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }
//
// void ABTPS_PlayerCharacter::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

void ABTPS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{	//컨트롤러가 구현되면 주석해제
		/*if(BTPS_PlayerController->MoveAction)
		{
			EnhancedInput->BindAction(
				BTPS_PlayerController->MoveAction,
				ETriggerEvent::Triggered,
				this,
				&ABTPS_PlayerCharacter::Move
			);
		}

		if (BTPS_PlayerController->LookAction)
		{
			EnhancedInput->BindAction(
				BTPS_PlayerController->LookAction,
				ETriggerEvent::Triggered,
				this,
				&ABTPS_PlayerCharacter::Look
			);
		}

		if (BTPS_PlayerController->JumpAction)
		{
			EnhancedInput->BindAction(
				BTPS_PlayerController->JumpAction,
				ETriggerEvent::Triggered,
				this,
				&ABTPS_PlayerCharacter::StartJump
			);
		}

		if (BTPS_PlayerController->JumpAction)
		{
			EnhancedInput->BindAction(
				BTPS_PlayerController->JumpAction,
				ETriggerEvent::Completed,
				this,
				&ABTPS_PlayerCharacter::StopJump
			);
		}

		if (BTPS_PlayerController->SprintAction)
		{
			EnhancedInput->BindAction(
				BTPS_PlayerController->SprintAction,
				ETriggerEvent::Triggered,
				this,
				&ABTPS_PlayerCharacter::StartSprint
			);
		}

		if (BTPS_PlayerController->SprintAction)
		{
			EnhancedInput->BindAction(
				BTPS_PlayerController->SprintAction,
				ETriggerEvent::Completed,
				this,
				&ABTPS_PlayerCharacter::StopSprint
			);
		}*/

	}
}


void ABTPS_PlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ABTPS_PlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);

	AddControllerPitchInput(LookInput.Y);
}

void ABTPS_PlayerCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ABTPS_PlayerCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ABTPS_PlayerCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABTPS_PlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
