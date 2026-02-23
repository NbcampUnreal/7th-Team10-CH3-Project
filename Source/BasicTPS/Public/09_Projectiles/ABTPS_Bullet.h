// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTPS_ProjectileBase.h"
#include "ABTPS_Bullet.generated.h"

UCLASS()
class BASICTPS_API AABTPS_Bullet : public ABTPS_ProjectileBase
{
	GENERATED_BODY()

public:

	AABTPS_Bullet();

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
