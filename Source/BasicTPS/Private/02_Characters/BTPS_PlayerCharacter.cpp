#include "02_Characters/BTPS_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ABTPS_PlayerCharacter::ABTPS_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// SpringArm->SetupAttachment(RootComponent);
	// SpringArm->TargetArmLength = 300.0f;
	// SpringArm->bUsePawnControllerRotation = true;
	//
	// CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// CameraComp->SetupAttachment(SpringArm);
	// CameraComp->bUsePawnControllerRotation = false;
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
}

