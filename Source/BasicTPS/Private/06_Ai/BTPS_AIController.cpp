// Fill out your copyright notice in the Description page of Project Settings.


#include "06_Ai/BTPS_AIController.h"
#include "02_Characters/BTPS_EnemyCharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ABTPS_AIController::ABTPS_AIController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;
		SightConfig->LoseSightRadius = 2000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		
		PerceptionComponent->ConfigureSense((*SightConfig));
		PerceptionComponent->SetDominantSense((SightConfig->GetSenseImplementation()));
		
	}
}

void ABTPS_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABTPS_AIController::OnTargetDetected);
	}
}

void ABTPS_AIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	auto* MyPawn = Cast<ABTPS_EnemyCharacterBase>(GetPawn());
	if (!MyPawn) return;
	if (Actor && Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller: Target Found! : %s"), *Actor->GetName());
		MyPawn->SetTarget(Actor);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AI Controller: Target Lost: %s"), *Actor->GetName());
		MyPawn->ClearTarget();
	}
}


