// Fill out your copyright notice in the Description page of Project Settings.


#include "06_Ai/BTPS_AIController.h"


// Sets default values
ABTPS_AIController::ABTPS_AIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABTPS_AIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTPS_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

