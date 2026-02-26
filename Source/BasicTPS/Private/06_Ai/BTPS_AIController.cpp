// Fill out your copyright notice in the Description page of Project Settings.


#include "06_Ai/BTPS_AIController.h"
#include "02_Characters/BTPS_EnemyCharacterBase.h"

#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"

ABTPS_AIController::ABTPS_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;
		SightConfig->LoseSightRadius = 2000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		
		PerceptionComponent->ConfigureSense(*SightConfig);
		PerceptionComponent->SetDominantSense((SightConfig->GetSenseImplementation()));
	}
	
	if (DamageConfig)
	{
		PerceptionComponent->ConfigureSense((*DamageConfig));
	}
}

void ABTPS_AIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABTPS_AIController::OnTargetDetected);
	}
}

void ABTPS_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ABTPS_EnemyCharacterBase* Enemy = Cast<ABTPS_EnemyCharacterBase>(InPawn);
	if (Enemy)
	{
		if (Enemy->GetBehaviorTree())
		{
			RunBehaviorTree(Enemy->GetBehaviorTree());
		}
	}
}

void ABTPS_AIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	auto* MyPawn = Cast<ABTPS_EnemyCharacterBase>(GetPawn());
	if (!MyPawn || !Actor) return;
	
	IGenericTeamAgentInterface* MyTeamAgent = Cast<IGenericTeamAgentInterface>(MyPawn);
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(Actor);

	if (MyTeamAgent && TargetTeamAgent)
	{
		if (MyTeamAgent->GetGenericTeamId() == TargetTeamAgent->GetGenericTeamId())
		{
			return; 
		}
	}
	
	UBlackboardComponent* BB = GetBlackboardComponent();
	
	if (Actor && Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Log, TEXT("[AI Controller] Target Found!: %s"), *Actor->GetName());
		MyPawn->SetTarget(Actor);
		
		if (BB)
		{
			BB->SetValueAsObject(TargetKeyName, Actor);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[AI Controller] Target Lost: %s"), *Actor->GetName());
		MyPawn->ClearTarget();
		
		if (BB)
		{
			BB->SetValueAsObject(TargetKeyName, nullptr);
		}
	}
}


