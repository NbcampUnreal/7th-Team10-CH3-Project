#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player.generated.h"

UCLASS()
class BASICTPS_API APlayer : public ABaseCharacter
{
	GENERATED_BODY()

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	float MaxStamina;
	float CurrentStamina;
	
	
public:
	APlayer();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	
	float HealHp(float HealAmount);
	
	virtual void AttackEnemy() override;
	virtual float TakeDamage( 
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;;
	virtual float ApplyDamage(float Damage) override;
	virtual bool IsDead(float CurrentHP) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
