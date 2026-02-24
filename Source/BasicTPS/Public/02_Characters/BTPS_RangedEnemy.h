// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTPS_EnemyCharacterBase.h"
#include "BTPS_RangedEnemy.generated.h"

UCLASS()
class BASICTPS_API ABTPS_RangedEnemy : public ABTPS_EnemyCharacterBase
{
	GENERATED_BODY()

public:
	ABTPS_RangedEnemy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AActor> ProjectileClass;


public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireProjectile();

};
