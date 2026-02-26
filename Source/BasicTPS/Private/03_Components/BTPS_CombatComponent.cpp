// Fill out your copyright notice in the Description page of Project Settings.


#include "03_Components/BTPS_CombatComponent.h"
#include "03_Components/BTPS_StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"
#include "02_Characters/BTPS_EnemyCharacterBase.h"

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
	
	if (Cast<ABTPS_EnemyCharacterBase>(DamagedActor))
	{
		APawn* InstigatorPawn = nullptr;
	
		if (InstigatedBy) 
		{
			InstigatorPawn = InstigatedBy->GetPawn();
		}
		else if (DamageCauser) 
		{
			InstigatorPawn = Cast<APawn>(DamageCauser->GetInstigator()); 
		
			if (!InstigatorPawn) InstigatorPawn = Cast<APawn>(DamageCauser); 
		}
	
		if (InstigatorPawn && DamagedActor)
		{
			UAISense_Damage::ReportDamageEvent(
			   GetWorld(),
			   DamagedActor,
			   InstigatorPawn, 
			   Damage,
			   InstigatorPawn->GetActorLocation(),
			   DamagedActor->GetActorLocation()
			);
			UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] AI Damage Report Target: %s"), *InstigatorPawn->GetName())
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[CombatComponent] AI Damage Report Target Not Found!"))
		}
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

