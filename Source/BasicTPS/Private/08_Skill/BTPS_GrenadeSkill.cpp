#include "08_Skill/BTPS_GrenadeSkill.h"
#include "02_Characters/BTPS_PlayerCharacter.h"

UBTPS_GrenadeSkill::UBTPS_GrenadeSkill()
{
	SkillCost = 20;
	SkillCoolTime = 8.f;
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

	FVector SpawnLocation = Mesh->GetSocketLocation("GrenadeSocket");
	FRotator SpawnRotation = OwnerCharacter->GetControlRotation();
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
}
