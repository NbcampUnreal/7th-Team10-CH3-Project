// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTPS/Public/Components/CombatComponent.h"
//#include "BasicTPS/Public/Components/StatComponent.h"
#include "Kismet/GameplayStatics.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false; 
}

 // void UCombatComponent::OnTakeDamage(float IncomingDamage)
 // {
 // 	if (StatComp)
 // 	{
 // 		StatComp->OnTakeDamage(IncomingDamage);
 // 		UE_LOG(LogTemp, Log, TEXT("Take Damage!: %f"), IncomingDamage);
 // 	}
 // }

void UCombatComponent::ExecuteAttack(AActor* TargetActor, float DamageAmount, FHitResult HitInfo)
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

