// Fill out your copyright notice in the Description page of Project Settings.


#include "09_Projectiles/BTPS_EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"


ABTPS_EnemyProjectile::ABTPS_EnemyProjectile()
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

void ABTPS_EnemyProjectile::OnHit(
	UPrimitiveComponent* HitComp, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpules,
	const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpules, Hit);
	
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		bool bCanDamage = true;
		IGenericTeamAgentInterface* MyTeamAgent = Cast<IGenericTeamAgentInterface>(GetInstigator());
		IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherActor);
       
		if (MyTeamAgent && OtherTeamAgent) 
		{
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
    
	// 3. 누구를 맞췄든, 벽에 맞았든 투사체는 파괴됩니다.
	Destroy();
}

