#include "Player.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


APlayer::APlayer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControllerRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControllerRotation = false;
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

