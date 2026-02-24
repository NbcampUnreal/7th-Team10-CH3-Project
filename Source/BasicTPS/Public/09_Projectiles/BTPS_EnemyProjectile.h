// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTPS_ProjectileBase.h"
#include "BTPS_EnemyProjectile.generated.h"

UCLASS()
class BASICTPS_API ABTPS_EnemyProjectile : public ABTPS_ProjectileBase
{
	GENERATED_BODY()

public:

	ABTPS_EnemyProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageAmount = 20.0f;
	
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpules,
		const FHitResult& Hit) override;
	
};
