#include "08_Skill/BTPS_GrenadeSkill.h"
#include "02_Characters/BTPS_PlayerCharacter.h"
#include "09_Projectiles/BTPS_GrenadeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

UBTPS_GrenadeSkill::UBTPS_GrenadeSkill()
{
	SkillCost = 20;
	SkillCoolTime = 8.f;
	UpwardForce = 500.f;
	RiseAngle = FRotator(0.f, 0.f, 20.f);
}

void UBTPS_GrenadeSkill::SkillActivation()
{
	//TODO_CSH 캐릭터 투척 애니메이션 구현 완료되면 호출구문 주석 해제 필요
	//PlayThrowAnimation();
	UBTPS_GrenadeSkill::SpawnGrenadeProjectile();
}

void UBTPS_GrenadeSkill::SpawnGrenadeProjectile()
{
	if (!GrenadeClass)
		return;

	ACharacter* OwnerCharacter = Cast<ABTPS_PlayerCharacter>(GetOwner());
	if (!OwnerCharacter)
		return;

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh)
		return;

	FVector SpawnLocation = Mesh->GetSocketLocation("GrenadeSocket")
		+ OwnerCharacter->GetActorForwardVector() * 50.f;
	FRotator SpawnRotation = OwnerCharacter->GetControlRotation()
		+ RiseAngle;
	FActorSpawnParameters Params;
	Params.Owner = OwnerCharacter;
	Params.Instigator = OwnerCharacter;

	ABTPS_GrenadeProjectile* Projectile =
		GetWorld()->SpawnActor<ABTPS_GrenadeProjectile>(
			GrenadeClass,
			SpawnLocation,
			SpawnRotation,
			Params
		);

	if (Projectile && Projectile->ProjectileMovement)
	{
		UProjectileMovementComponent* PMC = Projectile->ProjectileMovement;

		FVector LunchDir = GetOwner()->GetActorForwardVector() + FVector(0, 0, 0.7f);
		LunchDir.Normalize();

		FVector FinalVelocity = LunchDir * UpwardForce;

		PMC->InitialSpeed += FinalVelocity.Size();
		PMC->MaxSpeed = FMath::Max(PMC->MaxSpeed, PMC->InitialSpeed);

		PMC->Velocity += FinalVelocity;

		PMC->UpdateComponentVelocity();
	}
}
