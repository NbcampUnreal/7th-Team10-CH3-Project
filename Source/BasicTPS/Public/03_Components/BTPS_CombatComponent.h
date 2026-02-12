// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTPS_CombatComponent.generated.h"

class UBTPS_StatComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASICTPS_API UBTPS_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBTPS_CombatComponent();
	
protected:
	virtual void BeginPlay() override;
	
public:
	void ExecuteAttack(AActor* TargetActor, float DamageAmount, FHitResult& HitInfo);
	
private:
	UPROPERTY()
	TObjectPtr<UBTPS_StatComponent> StatComp;
	
	UFUNCTION()
	void OnDamageResponse(
		AActor* DamagedActor, 
		float Damage, 
		const UDamageType* DamageType, 
		AController* InstigatedBy, 
		AActor* DamageCauser
		);
};