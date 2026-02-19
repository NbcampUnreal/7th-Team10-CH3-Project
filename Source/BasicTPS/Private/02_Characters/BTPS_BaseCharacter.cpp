#include "02_Characters/BTPS_BaseCharacter.h"
#include "03_Components/BTPS_StatComponent.h"
#include "03_Components/BTPS_CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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