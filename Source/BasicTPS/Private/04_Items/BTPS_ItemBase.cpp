// Fill out your copyright notice in the Description page of Project Settings.


#include "04_Items/BTPS_ItemBase.h"


// Sets default values
ABTPS_ItemBase::ABTPS_ItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABTPS_ItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTPS_ItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

