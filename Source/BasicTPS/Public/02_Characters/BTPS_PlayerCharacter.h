#pragma once

#include "CoreMinimal.h"
#include "BTPS_BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BTPS_PlayerCharacter.generated.h"

UCLASS()
class BASICTPS_API ABTPS_PlayerCharacter : public ABTPS_BaseCharacter
{
	GENERATED_BODY()

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	float MaxStamina;
	float CurrentStamina;
	
	
public:
	ABTPS_PlayerCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	
	// float HealHp(float HealAmount);
	
	// virtual void AttackEnemy() override;
	// virtual float TakeDamage( 
	// 	float DamageAmount,
	// 	FDamageEvent const& DamageEvent,
	// 	AController* EventInstigator,
	// 	AActor* DamageCauser) override;;
	// virtual float ApplyDamage(float Damage) override;
	// virtual bool IsDead(float CurrentHP) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
