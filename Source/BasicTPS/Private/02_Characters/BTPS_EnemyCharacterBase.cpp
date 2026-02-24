#include "02_Characters/BTPS_EnemyCharacterBase.h"

#include "01_Game/BTPS_GameState.h"
#include "BrainComponent.h"
#include "03_Components/BTPS_StatComponent.h"
#include "06_Ai/BTPS_AIController.h"
#include "Components/CapsuleComponent.h"


ABTPS_EnemyCharacterBase::ABTPS_EnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	TeamID = FGenericTeamId(1);
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

void ABTPS_EnemyCharacterBase::OnDeath()
{
	Super::OnDeath();

	ABTPS_AIController* AIController = Cast<ABTPS_AIController>(GetController());
	if (AIController && AIController->GetBrainComponent())
	{
		AIController->StopMovement();
		AIController->GetBrainComponent()->StopLogic("Dead");
	}
	
	// 에너미 사망시 물리 시뮬
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetCollisionProfileName(TEXT("PhysicsActor"));
        
		Capsule->BodyInstance.bLockXRotation = false;
		Capsule->BodyInstance.bLockYRotation = false;
        
		Capsule->SetEnableGravity(true);
		Capsule->SetSimulatePhysics(true);
        
		FVector PushDirection = GetActorForwardVector() * -1.0f; // 캐릭터의 뒤쪽 방향
		Capsule->AddImpulse(PushDirection * 1500.0f, NAME_None, true); // 힘(1500)을 가함
	}
	
	if (ABTPS_GameState* GS = GetWorld()->GetGameState<ABTPS_GameState>())
	{
		GS->OnMonsterKilled(10);
	}
	
	SetLifeSpan(3.0f);
}
