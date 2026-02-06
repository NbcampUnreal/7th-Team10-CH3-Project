#include "02_Characters/BTPS_BaseCharacter.h"

ABTPS_BaseCharacter::ABTPS_BaseCharacter()
	: MaxHp(100),
	  CurrentHp(MaxHp),
	  AttackPower(10),
	  DefensePower(10),
	  bIsDead(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABTPS_BaseCharacter::Attack()
{
}

float ABTPS_BaseCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator,
	AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// float ABTPS_BaseCharacter::ApplyDamage(float Damage)
// {
//
// }

// void ABTPS_BaseCharacter::OnDeath()
// {
// }

void ABTPS_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABTPS_BaseCharacter::SetCurrentHp(float Amount)
{
	if ((CurrentHp += Amount) > MaxHp)
	{
		return;
	} else if (CurrentHp > 0)
	{
		CurrentHp += Amount;
		if (CurrentHp <= 0)
		{
			bIsDead = true;
		}
	}
}