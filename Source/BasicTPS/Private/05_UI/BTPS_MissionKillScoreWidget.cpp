// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_MissionKillScoreWidget.h"
#include "01_Game/BTPS_GameState.h"
#include "Kismet/GameplayStatics.h"

void UBTPS_MissionKillScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ABTPS_GameState* GS = Cast<ABTPS_GameState>(UGameplayStatics::GetGameState(this)))
	{
		GS->OnMissionScoreChanged.AddDynamic(this, &UBTPS_MissionKillScoreWidget::UpdateMissionScore);
        
		UpdateMissionScore(GS->KilledMonsterCount, GS->SpawnMonsterCount);
	}
}

void UBTPS_MissionKillScoreWidget::UpdateMissionScore(int32 killMonster, int32 SpawnMonster)
{
	if (!Text_MissionScore.Get())
	{
		return;
	}
		FString MissionScore = FString::Printf(TEXT("%d / %d "), killMonster, SpawnMonster);
		Text_MissionScore->SetText(FText::FromString(MissionScore));
}
