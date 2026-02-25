// Fill out your copyright notice in the Description page of Project Settings.


#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_GameInstance.h"
#include "01_Game/BTPS_SpawnManager.h"
#include "02_Characters/BTPS_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"


ABTPS_GameState::ABTPS_GameState()
{
	LevelDuration = 30.0f;
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

	// TODO: UI 업데이트
	UpdateHUD();

	if (SpawnMonsterCount > 0 && KilledMonsterCount >= SpawnMonsterCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level Cleared! All monsters are dead."));

		GetWorldTimerManager().ClearTimer(LevelTimerHandle);
		EndLevel();
	}
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
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
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

void ABTPS_GameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ABTPS_PlayerController* BTPS_PlayerController = Cast<ABTPS_PlayerController>(PlayerController))
		{
			BTPS_PlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UBTPS_GameInstance* BTPS_GameInstance = Cast<UBTPS_GameInstance>(GameInstance);
		if (BTPS_GameInstance)
		{
			CurrentLevelIndex = BTPS_GameInstance->CurrentLevelIndex;
		}
	}

	SpawnMonsterCount = 0;
	KilledMonsterCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABTPS_SpawnManager::StaticClass(), FoundVolumes);
	
	if (FoundVolumes.Num() > 0)
	{
		if (ABTPS_SpawnManager* Manager = Cast<ABTPS_SpawnManager>(FoundVolumes[0]))
		{
			SpawnMonsterCount = Manager->SpawnMultipleEnemies(TotalToSpawn);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Level %d Started. Total Monsters: %d"), CurrentLevelIndex + 1, SpawnMonsterCount);
	
	UpdateHUD();

	FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (CurrentMapName != TEXT("L_MenuLevel"))
	{
		GetWorldTimerManager().SetTimer(
			LevelTimerHandle,
			this,
			&ABTPS_GameState::OnLevelTimeUp,
			LevelDuration,
			false
		);
	}
}

void ABTPS_GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
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