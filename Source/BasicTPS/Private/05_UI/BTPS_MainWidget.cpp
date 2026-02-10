// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_MainWidget.h"
#include "03_Components/BTPS_StatComponent.h"
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
