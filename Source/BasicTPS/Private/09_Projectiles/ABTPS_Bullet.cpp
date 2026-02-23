// Fill out your copyright notice in the Description page of Project Settings.


#include "09_Projectiles/ABTPS_Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"


AABTPS_Bullet::AABTPS_Bullet()
{
	if (CollisionComp)
	{
		CollisionComp->SetSimulatePhysics(false);
	}
	
	if (ProjectileMovement)
	{
		ProjectileMovement->ProjectileGravityScale = 0.0f;
	}
}

void AABTPS_Bullet::OnHit(
	UPrimitiveComponent* HitComp, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpules,
	const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpules, Hit);
	
	bool bCanDamage = true;
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		IGenericTeamAgentInterface* MyTeamAgent = Cast<IGenericTeamAgentInterface>(GetInstigator());
		IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherActor);
		
		if (MyTeamAgent->GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId())
		{
			bCanDamage = false;
		}
	}
	
	if (bCanDamage)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			DamageAmount,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Warning, TEXT("[Bullet] %s hit %s! Damage: %f"), 
			*GetName(), *OtherActor->GetName(), DamageAmount);
	}
}

