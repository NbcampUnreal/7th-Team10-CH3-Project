// Fill out your copyright notice in the Description page of Project Settings.


#include "02_Characters/BTPS_RangedEnemy.h"

ABTPS_RangedEnemy::ABTPS_RangedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ABTPS_RangedEnemy::FireProjectile()
{
	if (ProjectileClass && HasTarget())
	{
		FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f); 
		
		AActor* TargetActor = GetTarget();
		if (!TargetActor) return;
		
		FVector TargetLocation = TargetActor->GetActorLocation();
		FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}
