// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_MainWidget.h"
#include "01_Game/BTPS_GameState.h"
#include "03_Components/BTPS_StatComponent.h"
#include "03_Components/BTPS_ShootingMachineComponent.h"
#include "04_Items/A_Equipment/Weapon/BTPS_WeaponBase.h"
#include "Components/ProgressBar.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "TimerManager.h"

void UBTPS_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimer(
	RetryBindHandle,
	this,
	&UBTPS_MainWidget::TickReadTime,
	0.1f,
	true
);
}

void UBTPS_MainWidget::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RetryBindHandle);
		GetWorld()->GetTimerManager().ClearTimer(KillMarkerTimerHandle);
	}

	Super::NativeDestruct();
}

void UBTPS_MainWidget::TickReadTime()
{
	auto* GS = GetWorld() ? GetWorld()->GetGameState<ABTPS_GameState>() : nullptr;
	if (!GS) return;

	UpdateLevelDuration();
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
	
	if (HPText)
	{
		if (MaxHP > 0.0f)
		{
			float Percent = CurrentHP / MaxHP;
			HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));

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
	
	if (StaminaText)
	{
		if (MaxStamina > 0.0f)
		{
			float Percent =  CurrentStamina / MaxStamina;
			StaminaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentStamina, MaxStamina)));

		}
	}
}

void UBTPS_MainWidget::BindShootingComp(UBTPS_ShootingMachineComponent* ShootingMachineComp)
{
	if (ShootingMachineComp)
	{
		ShootingMachineComp->OnAmmoChanged.AddDynamic(this, &UBTPS_MainWidget::UpdateAmmo);
		ShootingMachineComp->OnAimTargetChanged.AddDynamic(this, &UBTPS_MainWidget::UpdateCrosshairColor);

		if (ShootingMachineComp->CurrentWeapon)
		{
			UpdateAmmo(ShootingMachineComp->CurrentWeapon->GetCurrentAmmo(), 
				ShootingMachineComp->CurrentWeapon->GetMaxAmmo(),
				ShootingMachineComp->CurrentWeapon->GetReserveAmmo()
			);
		}
	}
}

// void UBTPS_MainWidget::SetWeaponImage(UTexture2D* NewTexture)
// {
// 	if (!WeaponImage || !NewTexture)
// 		return;
// 	WeaponImage->UImage::SetBrushFromTexture(NewTexture);
// }

void UBTPS_MainWidget::ShowWeaponUI()
{
	if (WeaponInfoBox)
	{
		WeaponInfoBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBTPS_MainWidget::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo, int32 ReserveAmmo)
{
	if (Text_AmmoInfo.Get())
	{
		FString AmmoString = FString::Printf(TEXT("%d / %d (%d)"), CurrentAmmo, MaxAmmo, ReserveAmmo);
		Text_AmmoInfo->SetText(FText::FromString(AmmoString));

		ShowWeaponUI();
	}
}

void UBTPS_MainWidget::UpdateCrosshairColor(bool bIsAimingAtEnemy)
{
	if (!Crosshair) return;

	if (bIsAimingAtEnemy)
	{
		Crosshair->SetBrushColor(FLinearColor::Red);
	}
	else
	{
		Crosshair->SetBrushColor(FLinearColor::White);
	}
}

void UBTPS_MainWidget::ShowKillMarker()
{
	if (!Image_KillMarker) return;

	Image_KillMarker->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	GetWorld()->GetTimerManager().SetTimer(
		KillMarkerTimerHandle,
		this,
		&UBTPS_MainWidget::HideKillMarker,
		0.5f,
		false
	);
}

void UBTPS_MainWidget::HideKillMarker()
{
	if (Image_KillMarker)
	{
		Image_KillMarker->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBTPS_MainWidget::UpdateLevelDuration()
{
	auto* GS = GetWorld() ? GetWorld()->GetGameState<ABTPS_GameState>() : nullptr;
	if (!GS) return;

	if (!IsValid(Timer)) return;

	const float RemainingTime = GS->GetLevelRemainingTime();

	Timer->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
}