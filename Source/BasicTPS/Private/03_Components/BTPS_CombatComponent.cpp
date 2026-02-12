// Fill out your copyright notice in the Description page of Project Settings.


#include "03_Components/BTPS_CombatComponent.h"
#include "03_Components/BTPS_StatComponent.h"
#include "Kismet/GameplayStatics.h"

UBTPS_CombatComponent::UBTPS_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false; 
}

void UBTPS_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UBTPS_CombatComponent::OnDamageResponse);
		StatComp = Owner->FindComponentByClass<UBTPS_StatComponent>();
	}
}

void UBTPS_CombatComponent::OnDamageResponse(
	AActor* DamagedActor, 
	float Damage, 
	const UDamageType* DamageType, 
	AController* InstigatedBy, 
	AActor* DamageCauser)
 {
	if (Damage <= KINDA_SMALL_NUMBER) return;
 	if (StatComp)
 	{
 		StatComp->OnTakeDamage(Damage);
 		UE_LOG(LogTemp, Log, TEXT("CombatComponent: Take Damage!: %f"), Damage);
 	}
 }

void UBTPS_CombatComponent::ExecuteAttack(AActor* TargetActor, float DamageAmount, FHitResult& HitInfo)
{
	if (!TargetActor) return;
	
	UGameplayStatics::ApplyDamage(
		TargetActor,
		DamageAmount,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		UDamageType::StaticClass()
	);
	UE_LOG(LogTemp, Log, TEXT("CombatComponent: Damage To Target!: %s, Damage: %f"), *TargetActor->GetName(), DamageAmount);
}

