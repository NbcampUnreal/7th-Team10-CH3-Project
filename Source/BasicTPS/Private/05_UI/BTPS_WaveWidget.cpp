// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_WaveWidget.h"
#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_WaveManager.h"
#include "Kismet/GameplayStatics.h"

void UBTPS_WaveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ABTPS_GameState* GS = Cast<ABTPS_GameState>(UGameplayStatics::GetGameState(this)))
	{
		if (GS->WaveManager)
		{
			GS->WaveManager->OnWaveChanged.AddDynamic(this, &UBTPS_WaveWidget::UpdateWaveText);

			int32 CurrentWaveNum = GS->WaveManager->GetCurrentWaveIndex() + 1;
			int32 TotalWaveNum = GS->WaveManager->GetTotalWaves();
            
			UpdateWaveText(CurrentWaveNum, TotalWaveNum);
		}
	}
}

void UBTPS_WaveWidget::UpdateWaveText(int32 CurrentWave, int32 TotalWaves)
{
	if (!Text_CurrentWave) return;

	FString WaveString = FString::Printf(TEXT("%d / %d"), CurrentWave, TotalWaves);
    
	Text_CurrentWave->SetText(FText::FromString(WaveString));
}
