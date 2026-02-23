// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTPS_Bullet.generated.h"

UCLASS()
class BASICTPS_API ABTPS_Bullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABTPS_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* MovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* CollisionComp;
	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* BulletMeshComp;
	
	void Die();
	
	
};
