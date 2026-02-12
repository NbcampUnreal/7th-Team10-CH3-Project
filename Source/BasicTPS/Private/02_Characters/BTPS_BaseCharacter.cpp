#include "02_Characters/BTPS_BaseCharacter.h"
#include "03_Components/BTPS_StatComponent.h"
#include "03_Components/BTPS_CombatComponent.h"


ABTPS_BaseCharacter::ABTPS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StatComp = CreateDefaultSubobject<UBTPS_StatComponent>(TEXT("StatComp"));
	CombatComp = CreateDefaultSubobject<UBTPS_CombatComponent>(TEXT("CombatComp"));
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
