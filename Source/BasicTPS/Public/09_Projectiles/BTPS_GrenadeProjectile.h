#pragma once

#include "CoreMinimal.h"
#include "09_Projectiles/BTPS_ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "BTPS_GrenadeProjectile.generated.h"

UCLASS()
class BASICTPS_API ABTPS_GrenadeProjectile : public ABTPS_ProjectileBase
{
	GENERATED_BODY()

public:
	ABTPS_GrenadeProjectile();
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade")
	float ExplosionDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> ExplosionComp;

protected:
	virtual void BeginPlay() override;\

	void Explode();

	float FuseTime = 3.f;

	FTimerHandle FuseTimerHandle;

	bool bExploded = false;

};
