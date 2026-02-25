// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_EnemyHealthBarWidget.h"
#include "03_Components/BTPS_StatComponent.h"
#include "Components/ProgressBar.h"

void UBTPS_EnemyHealthBarWidget::BindStatComp(UBTPS_StatComponent* StatComp)
{
	if (StatComp)
	{
		UpdateHealthBar(StatComp->GetCurrentHP(), StatComp->GetMaxHP());
		StatComp->OnHPChanged.AddDynamic(this, &UBTPS_EnemyHealthBarWidget::UpdateHealthBar);
	}
}

void UBTPS_EnemyHealthBarWidget::UpdateHealthBar(float CurrentHP, float MaxHP)
{
	if (HPProgressBar && MaxHP > 0.f)
	{
		HPProgressBar->SetPercent(CurrentHP / MaxHP);
	}
}

