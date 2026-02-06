// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTPS/Public/03_Components/BTPS_CombatComponent.h"
//#include "BasicTPS/Public/Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"

UBTPS_CombatComponent::UBTPS_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false; 
}

 // void UBTPS_CombatComponent::OnTakeDamage(float IncomingDamage)
 // {
 // 	if (StatComp)
 // 	{
 // 		StatComp->OnTakeDamage(IncomingDamage);
 // 		UE_LOG(LogTemp, Log, TEXT("Take Damage!: %f"), IncomingDamage);
 // 	}
 // }

void UBTPS_CombatComponent::ExecuteAttack(AActor* TargetActor, float DamageAmount, FHitResult HitInfo)
{
	if (!TargetActor) return;
	
	UGameplayStatics::ApplyDamage(
		TargetActor,
		DamageAmount,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		UDamageType::StaticClass()
	);
	UE_LOG(LogTemp, Log, TEXT("Damage To Target!: %s, Damage: %f"), *TargetActor->GetName(), DamageAmount);
}

