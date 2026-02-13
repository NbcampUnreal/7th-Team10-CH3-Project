#include "02_Characters/BTPS_EnemyCharacterBase.h"

#include "06_Ai/BTPS_AIController.h"

ABTPS_EnemyCharacterBase::ABTPS_EnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AIControllerClass = ABTPS_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABTPS_EnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABTPS_EnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABTPS_EnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABTPS_EnemyCharacterBase::SetTarget(AActor* NewTarget)
{
	if (!IsValid(NewTarget)) return;

	if (NewTarget == this) return;

	CurrentTarget = NewTarget;

	//AI 관련 추가 시 타겟관련 행동 추가
}

AActor* ABTPS_EnemyCharacterBase::GetTarget() const
{
	return CurrentTarget.Get();
}

bool ABTPS_EnemyCharacterBase::HasTarget() const
{
	return CurrentTarget.IsValid();
}

void ABTPS_EnemyCharacterBase::ClearTarget()
{
	CurrentTarget = nullptr;
}