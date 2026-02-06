#include "02_Characters/BTPS_EnemyCharacter.h"

ABTPS_EnemyCharacter::ABTPS_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABTPS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABTPS_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABTPS_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

