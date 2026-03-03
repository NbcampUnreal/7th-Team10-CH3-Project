// Fill out your copyright notice in the Description page of Project Settings.


#include "06_Ai/BTPS_BTTask_FireProjectile.h"
#include "AIController.h"
#include "02_Characters/BTPS_RangedEnemy.h"

UBTPS_BTTask_FireProjectile::UBTPS_BTTask_FireProjectile()
{
	NodeName = TEXT("Fire Projectile");
}

EBTNodeResult::Type UBTPS_BTTask_FireProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		ABTPS_RangedEnemy* RangedEnemy = Cast<ABTPS_RangedEnemy>(AIController->GetPawn());
		if (RangedEnemy)
		{
			RangedEnemy->FireProjectile();
			RangedEnemy->StartAttackCooldown();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}