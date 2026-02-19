// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_MainWidget.h"
#include "03_Components/BTPS_StatComponent.h"
#include "03_Components/BTPS_ShootingMachineComponent.h"
#include "04_Items/A_Equipment/Weapon/BTPS_WeaponBase.h"
#include "Components/ProgressBar.h"


void UBTPS_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBTPS_MainWidget::BindStatComp(UBTPS_StatComponent* StatComp)
{
	if (StatComp)
	{
		UpdateHP(StatComp->GetCurrentHP(), StatComp->GetMaxHP());
		UpdateStamina(StatComp->GetCurrentStamina(), StatComp->GetMaxStamina());
		
		StatComp->OnHPChanged.AddDynamic(this, &UBTPS_MainWidget::UpdateHP);
		StatComp->OnStaminaChanged.AddDynamic(this, &UBTPS_MainWidget::UpdateStamina);
	}
}

void UBTPS_MainWidget::UpdateHP(float CurrentHP, float MaxHP)
{
	if (HPBar)
	{
		if (MaxHP > 0.0f)
		{
			float Percent = CurrentHP / MaxHP;
			HPBar->SetPercent(Percent);
		}
	}
}

void UBTPS_MainWidget::UpdateStamina(float CurrentStamina, float MaxStamina)
{
	if (StaminaBar)
	{
		if (MaxStamina > 0.0f)
		{
			float Percent = CurrentStamina / MaxStamina;
			StaminaBar->SetPercent(Percent);
		}
	}
}

void UBTPS_MainWidget::BindShootingComp(UBTPS_ShootingMachineComponent* ShootingMachineComp)
{
	if (ShootingMachineComp)
	{
		ShootingMachineComp->OnAmmoChanged.AddDynamic(this, &UBTPS_MainWidget::UpdateAmmo);

		if (ShootingMachineComp->CurrentWeapon)
		{
			UpdateAmmo(ShootingMachineComp->CurrentWeapon->GetCurrentAmmo(), ShootingMachineComp->CurrentWeapon->GetMaxAmmo());
		}
	}
}

void UBTPS_MainWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	//AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
}
