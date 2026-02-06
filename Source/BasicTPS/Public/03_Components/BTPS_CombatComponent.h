// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTPS_CombatComponent.generated.h"

//class UStatComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASICTPS_API UBTPS_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBTPS_CombatComponent();
	
protected:
	//virtual void BeginPlay() override;
	
public:
	void ExecuteAttack(AActor* TargetActor, float DamageAmount, FHitResult HitInfo);
//	void OnTakeDamage(float IncomingDamage);
	
//private:
//	UPROPERTY()
//	TObjectPtr<UStatComponent> StatComp;
//
};