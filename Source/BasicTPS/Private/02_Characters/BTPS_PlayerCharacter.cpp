#include "02_Characters/BTPS_PlayerCharacter.h"
#include "02_Characters/BTPS_PlayerController.h"
#include "03_Components/BTPS_ShootingMachineComponent.h"
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

	ShootingMachineComp = CreateDefaultSubobject<UBTPS_ShootingMachineComponent>(TEXT("ShootingMachine"));

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
	{	
		
		if (ABTPS_PlayerController* PlayerController = Cast<ABTPS_PlayerController>(GetController()))
		{
			if(PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ABTPS_PlayerCharacter::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ABTPS_PlayerCharacter::Look
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ABTPS_PlayerCharacter::StartJump
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ABTPS_PlayerCharacter::StopJump
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ABTPS_PlayerCharacter::StartSprint
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ABTPS_PlayerCharacter::StopSprint
				);
			}	

			if (ShootingMachineComp)
			{
				if (PlayerController->AimAction)
				{
					//헤더파일 UBTPS_ShootingMachineComponent* ShootingMachineComp; TObjectPtr<>로 바꾸지 마세요
					//BindAction parameter자체가 TObjectPtr<>를 못받아서 action binding이 안되요.
					EnhancedInput->BindAction(
						PlayerController->AimAction,
						ETriggerEvent::Started,
						ShootingMachineComp,
						&UBTPS_ShootingMachineComponent::AimStarted
					);
					EnhancedInput->BindAction(
						PlayerController->AimAction,
						ETriggerEvent::Completed,
						ShootingMachineComp,
						&UBTPS_ShootingMachineComponent::AimCompleted
					);
				}
				if (PlayerController->FireAction)
				{
					EnhancedInput->BindAction(
						PlayerController->FireAction,
						ETriggerEvent::Started,
						ShootingMachineComp,
						&UBTPS_ShootingMachineComponent::Fire
					);
				}
				if (PlayerController->InteractAction)
				{
					EnhancedInput->BindAction(
						PlayerController->InteractAction,
						ETriggerEvent::Started,
						ShootingMachineComp,
						&UBTPS_ShootingMachineComponent::Interact
					);
				}
				if (PlayerController->ToggleCameraAction)
				{
					EnhancedInput->BindAction(
						PlayerController->ToggleCameraAction,
						ETriggerEvent::Started,
						ShootingMachineComp,
						&UBTPS_ShootingMachineComponent::ToggleCamera
					);
				}
			}
		}
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
