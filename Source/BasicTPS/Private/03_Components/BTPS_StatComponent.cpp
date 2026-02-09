// Fill out your copyright notice in the Description page of Project Settings.


#include "03_Components/BTPS_StatComponent.h"


UBTPS_StatComponent::UBTPS_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bWantsInitializeComponent = true;
}

void UBTPS_StatComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	CurrentStamina = MaxStamina;
}

void UBTPS_StatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	RecoverStamina(StaminaRegenRate * DeltaTime);
	if (CurrentStamina >= MaxStamina)
	{
		SetComponentTickEnabled(false);
	}
}


void UBTPS_StatComponent::OnTakeDamage(float DamageAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.0f, MaxHP);
	if (OnHPChanged.IsBound())
	{
		OnHPChanged.Broadcast(CurrentHP, MaxHP);
	}
	
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
	}
}

void UBTPS_StatComponent::Heal(float HealAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0.0f, MaxHP);
	if (OnHPChanged.IsBound())
	{
		OnHPChanged.Broadcast(CurrentHP, MaxHP);
	}
}

bool UBTPS_StatComponent::bUseStamina(float Cost)
{
	if (CurrentStamina < Cost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Enough Stamina!"));
		return false;
	}
	
	CurrentStamina = FMath::Clamp(CurrentStamina - Cost, 0.0f, MaxStamina);
	StartRegenTimer();
	
	if (OnStaminaChanged.IsBound())
	{
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}
	
	return true;
}

void UBTPS_StatComponent::RegenStamina()
{	
	SetComponentTickEnabled(true);
}

void UBTPS_StatComponent::RecoverStamina(float RecoverAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + RecoverAmount, 0.0f, MaxStamina);
	if (OnStaminaChanged.IsBound())
	{
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}
}

void UBTPS_StatComponent::StartRegenTimer()
{
	SetComponentTickEnabled(false);

	GetWorld()->GetTimerManager().SetTimer(
		StaminaRegenTimerHandle,
		this,
		&UBTPS_StatComponent::RegenStamina,
		StaminaRegenDelay,
		false
	);
}


