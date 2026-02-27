#include "02_Characters/BTPS_BaseCharacter.h"
#include "03_Components/BTPS_StatComponent.h"
#include "03_Components/BTPS_CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h" 
#include "Kismet/GameplayStatics.h"   
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraTextToolkitHelpers.h"

ABTPS_BaseCharacter::ABTPS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StatComp = CreateDefaultSubobject<UBTPS_StatComponent>(TEXT("StatComp"));
	CombatComp = CreateDefaultSubobject<UBTPS_CombatComponent>(TEXT("CombatComp"));
	
	TeamID = FGenericTeamId(0);
}

void ABTPS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetStatComp())
	{
		GetStatComp()->OnDeath.AddDynamic(this, &ABTPS_BaseCharacter::OnDeath);
		
		GetStatComp()->OnDamageReceived.AddDynamic(this, &ABTPS_BaseCharacter::ShowDamageText);
	}
}


void ABTPS_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABTPS_BaseCharacter::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABTPS_BaseCharacter::ShowDamageText(float DamageAmount)
{
	if (DamageTextSystem) 
	{
		// 1. 기본 위치 (캐릭터 중심 + 위로 150)
		FVector BaseLoc = GetActorLocation() + DamageTextZOffset;
		FRotator LookAtRot = FRotator::ZeroRotator;
		FVector CameraRight = FVector::RightVector; // 기본값
	
		APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CamManager)
		{
			// 카메라 방향 구하기 (텍스트 회전용)
			LookAtRot = UKismetMathLibrary::FindLookAtRotation(BaseLoc, CamManager->GetCameraLocation());
           
			CameraRight = UKismetMathLibrary::GetRightVector(CamManager->GetCameraRotation());
		}

		// 2. 최종 스폰 위치 = 기본 위치 + (카메라 오른쪽 방향 * 50)
		FVector SpawnLoc = BaseLoc + (CameraRight * DamageTextYOffset);

		// 3. 계산된 회전값(LookAtRot)을 넣어 스폰합니다.
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), 
			DamageTextSystem, 
			SpawnLoc, 
			LookAtRot // <- 여기에 회전값 적용
		);
        
		if (NiagaraComp)
		{
			FString DamageStr = FString::Printf(TEXT("%.0f"), DamageAmount);
			UNiagaraTextToolkitHelpers::SetNiagaraNTTTextVariable(NiagaraComp, DamageStr);
		}
	}
}

FGenericTeamId ABTPS_BaseCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void ABTPS_BaseCharacter::OnDeath()
{
	if (bIsDead) return;
	bIsDead = true;
	
	SetCanBeDamaged(false);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	
}