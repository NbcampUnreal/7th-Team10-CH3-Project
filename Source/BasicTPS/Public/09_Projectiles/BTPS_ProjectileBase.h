#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BTPS_ProjectileBase.generated.h"

UCLASS()
class BASICTPS_API ABTPS_ProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABTPS_ProjectileBase();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	float CollisionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	float CollisionInitialSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	float CollisionMaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	float CollisionGravityScale;


	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

protected:
	virtual void BeginPlay() override;
};
