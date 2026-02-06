#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class BASICTPS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
private:
	float MaxHp;
	float CurrentHp;
	float Attack;
	float Defense;
	bool bIsDead;
	
public:	
	virtual void AttackEnemy() override;
	virtual float TakeDamage( 
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;;
	virtual float ApplyDamage(float Damage) override;
	virtual void OnDeath() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	float GetMaxHp() const {return MaxHp;}
	float GetCurrentHp() const {return CurrentHp;}
	float GetAttack() const {return Attack;}
	float GetDefense() const {return Defense;}
	
	void SetMaxHp(float IncreaseAmount){MaxHp += IncreaseAmount;}
	void SetCurrentHp(float Amount); // Increase, Decrease 둘다 가능
	void SetAttack(float IncreaseAmount){Attack += IncreaseAmount;}
	void SetDefence(float IncreaseAmount){Defense += IncreaseAmount;}
};
