#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
	: MaxHp(100),
	  CurrentHp(MaxHp),
	  Attack(10),
	  Defense(10),
	  bIsDead(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseCharacter::Attack()
{
}

float ABaseCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator,
	AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

float ABaseCharacter::ApplyDamage(float Damage)
{
}

void ABaseCharacter::OnDeath()
{
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::SetCurrentHp(float Amount)
{
	if ((CurrentHp += Amount) > MaxHp)
	{
		return;
	} else if (CurrentHp > 0)
	{
		CurrentHp += HealAmount;
		if (CurrentHp <= 0)
		{
			bIsDead = true;
		}
	}
}