// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_WaveAlertWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UBTPS_WaveAlertWidget::PlayWaveStartAlert(int32 WaveLevel)
{
	if (WaveStartText)
	{
		FString WaveMsg = FString::Printf(TEXT("WAVE %d START"), WaveLevel);
		WaveStartText->SetText(FText::FromString(WaveMsg));

		SetVisibility(ESlateVisibility::Visible);
	}

	if (WaveStartAnim)
	{
		PlayAnimation(WaveStartAnim);
	}
}

float UBTPS_WaveAlertWidget::GetWaveAlertDuration() const
{
	if (WaveStartAnim)
	{
		return WaveStartAnim->GetEndTime();
	}
	return 3.0f;
}