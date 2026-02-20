#pragma once

#include "CoreMinimal.h"
#include "BTPS_BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "08_Skill/BTPS_SkillComponent.h"
#include "08_Skill/BTPS_GrenadeSkill.h"
#include "BTPS_PlayerCharacter.generated.h"

struct FInputActionValue;
class UBTPS_ShootingMachineComponent;

UCLASS()
class BASICTPS_API ABTPS_PlayerCharacter : public ABTPS_BaseCharacter
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void ThrowGrenade(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintCostPerSecond = 15.0f; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float JumpStaminaCost = 20.0f;
	
private:
	float MaxStamina;
	float CurrentStamina;
	
	
public:
	ABTPS_PlayerCharacter();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	//UBTPS_ShootingMachineComponent* ShootingMachineComp; TObjectPtr<>로 바꾸지 마세요
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBTPS_ShootingMachineComponent* ShootingMachineComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBTPS_GrenadeSkill> GranadeSkillComp;
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
