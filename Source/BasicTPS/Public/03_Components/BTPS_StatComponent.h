// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTPS_StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChangedDelegate, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedDelegate, float, CurrentStamina, float, MaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASICTPS_API UBTPS_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBTPS_StatComponent();
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnHPChangedDelegate OnHPChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnStaminaChangedDelegate OnStaminaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Stat")
	FOnDeathDelegate OnDeath;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHP = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Defense = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|System")
	float HalfDamageDefense = 50.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Health")
	float CurrentHP;
	
public:	
	UFUNCTION(BlueprintPure, Category = "Stat|Health")
	float GetMaxHP() const {return MaxHP;}
	
	UFUNCTION(BlueprintPure, Category = "Stat|Health")
	float GetCurrentHP() const {return CurrentHP;}
	
	UFUNCTION(BlueprintCallable, Category = "Stat|Health")
	void OnTakeDamage(float DamageAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Stat|Health")
	void Heal(float HealAmount);
	
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStamina = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat|Stamina")
	float CurrentStamina;
	
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina")
	float StaminaRegenRate = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina")
	float StaminaRegenDelay = 1.0f;
	
	FTimerHandle StaminaRegenTimerHandle;
	
public:	
	UFUNCTION(BlueprintPure, Category = "Stat|Stamina")
	float GetMaxStamina() const {return MaxStamina;}
	
	UFUNCTION(BlueprintPure, Category = "Stat|Stamina")
	float GetCurrentStamina() const {return CurrentStamina;}
	
	UFUNCTION(BlueprintCallable, Category = "Stat|Stamina")
	bool TryUseStamina(float Cost);
	
	void RegenStamina();
	void RecoverStamina(float RecoverAmount);

private:
	void StartRegenTimer();
};
