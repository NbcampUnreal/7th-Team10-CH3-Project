#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BTPS_BaseCharacter.generated.h"

UCLASS()
class BASICTPS_API ABTPS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABTPS_BaseCharacter();
	
private:
	float MaxHp;
	float CurrentHp;
	float AttackPower; // Attack함수명이랑 같아서 빌드가 안되길래 잠시 변경했어요!
	float DefensePower; // <-too
	bool bIsDead;
	
public:	
	//빌드 테스트 하려고 주석처리했습니다!
	// virtual void AttackEnemy() override;
	virtual float TakeDamage( 
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;;
	//virtual float ApplyDamage(float Damage) override;
	// virtual void OnDeath() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	float GetMaxHp() const {return MaxHp;}
	float GetCurrentHp() const {return CurrentHp;}
	float GetAttack() const {return AttackPower;}
	float GetDefense() const {return DefensePower;}
	
	void SetMaxHp(float IncreaseAmount){MaxHp += IncreaseAmount;}
	void SetCurrentHp(float Amount); // Increase, Decrease 둘다 가능
	void SetAttack(float IncreaseAmount){AttackPower += IncreaseAmount;}
	void SetDefence(float IncreaseAmount){DefensePower += IncreaseAmount;}
	
	void Attack();
};
