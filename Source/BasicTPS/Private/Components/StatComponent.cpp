// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatComponent.h"


UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;
}

void UStatComponent::OnTakeDamage(float DamageAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.0f, MaxHP);
	UE_LOG(LogTemp, Log, TEXT("Loss HP! : %f / %f"), CurrentHP, MaxHP);
}
