// Fill out your copyright notice in the Description page of Project Settings.

#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_GameInstance.h"
#include "01_Game/BTPS_WaveManager.h"
#include "01_Game/BTPS_SpawnManager.h"
#include "02_Characters/BTPS_PlayerController.h"
#include "Kismet/GameplayStatics.h"


/*
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
*/

ABTPS_GameState::ABTPS_GameState()
{
	CurrentLevelIndex = 0;
	MaxLevels = 1;
	TotalToSpawn = 15;
}

void ABTPS_GameState::BeginPlay()
{
	Super::BeginPlay();
	StartLevel();
}

ABTPS_GameState* ABTPS_GameState::Get(const UObject* WorldContext)
{
	if (WorldContext == nullptr)
	{
		return nullptr;
	}

	UWorld* World = WorldContext->GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}

	return World->GetGameState<ABTPS_GameState>();
}


int32 ABTPS_GameState::GetScore() const
{
	return LevelScore;
}

void ABTPS_GameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UBTPS_GameInstance* BTPS_GameInstance = Cast<UBTPS_GameInstance>(GameInstance);
		if (BTPS_GameInstance)
		{
			BTPS_GameInstance->AddToScore(Amount);
		}
	}
}

void ABTPS_GameState::OnMonsterKilled(int32 ScoreReward)
{
	KilledMonsterCount++;
	LevelScore += ScoreReward;

	UE_LOG(LogTemp, Log, TEXT("Monster Killed! (%d / %d)"), KilledMonsterCount, SpawnMonsterCount);
	OnMissionScoreChanged.Broadcast(KilledMonsterCount, SpawnMonsterCount);
	
	AddScore(ScoreReward);
	
	/*
	UpdateHUD();
	*/

	if (WaveManager)
	{
		WaveManager->OnEnemyDead();
	}
}

void ABTPS_GameState::AddSpawnCount()
{
	SpawnMonsterCount++;
	OnMissionScoreChanged.Broadcast(KilledMonsterCount, SpawnMonsterCount);
}

float ABTPS_GameState::GetLevelRemainingTime() const
{
	if (WaveManager)
	{
		float RemainingTime = WaveManager->GetWaveRemainingTime();
		return FMath::Max(0.f, RemainingTime);
	}
	return 0.f;
}

void ABTPS_GameState::AddKillLog(FText EnemyName)
{
	FKillLogData NewLog;
	
	NewLog.KillerName = TEXT("Player");
	NewLog.VictimName = EnemyName.ToString();
	
	KillLogs.Add(NewLog);
	LastKillLogData = NewLog;
	OnKillLogAdded.Broadcast(EnemyName);
}

void ABTPS_GameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ABTPS_PlayerController* BTPS_PlayerController = Cast<ABTPS_PlayerController>(PlayerController))
		{
			BTPS_PlayerController->SetPause(true);
			BTPS_PlayerController->ShowGameOverMenu(true);
		}
	}
}

void ABTPS_GameState::OnLevelTimeUp()
{
	if (ABTPS_GameState* BTPSGameState = Cast<ABTPS_GameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		BTPSGameState->OnGameOver();
	}
}


/*
void ABTPS_GameState::UpdateHUD()
{
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ABTPS_PlayerController* BTPS_PlayerController = Cast<ABTPS_PlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = BTPS_PlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = 0.0f;
					if (WaveManager)
					{
						RemainingTime = WaveManager->GetWaveRemainingTime();
					}
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UBTPS_GameInstance* BTPS_GameInstance = Cast<UBTPS_GameInstance>(GameInstance);
						if (BTPS_GameInstance)
						{
							ScoreText->SetText(
								FText::FromString(FString::Printf(TEXT("Score: %d"), BTPS_GameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(
						FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
	
}
*/


void ABTPS_GameState::StartLevel()
{
	
	/*
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ABTPS_PlayerController* BTPS_PlayerController = Cast<ABTPS_PlayerController>(PlayerController))
		{
			BTPS_PlayerController->ShowGameHUD();
		}
	}
	*/

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UBTPS_GameInstance* BTPS_GameInstance = Cast<UBTPS_GameInstance>(GameInstance);
		if (BTPS_GameInstance)
		{
			CurrentLevelIndex = BTPS_GameInstance->CurrentLevelIndex;
		}
	}

	WaveManager = NewObject<UBTPS_WaveManager>(this);
	if (WaveManager)
	{
		WaveManager->Init(this);
		WaveManager->StartWave(0);
	}
	
	//UE_LOG(LogTemp, Log, TEXT("Level %d Started. Total Monsters: %d"), CurrentLevelIndex + 1, SpawnMonsterCount);
	
	/*
	UpdateHUD();
	*/

	FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

}

void ABTPS_GameState::EndLevel()
{
	AddScore(LevelScore);
	CurrentLevelIndex++;

	// 모든 레벨을 다 돌았다면 게임 오버 처리
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameClear();
		return;
	}

	// 레벨 맵 이름이 있다면 해당 맵 불러오기
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameClear();
	}
}

void ABTPS_GameState::OnGameClear()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ABTPS_PlayerController* BTPS_PlayerController = Cast<ABTPS_PlayerController>(PlayerController))
		{
			BTPS_PlayerController->ShowGameClearMenu();
		}
	}
}