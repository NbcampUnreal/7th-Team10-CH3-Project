// Fill out your copyright notice in the Description page of Project Settings.

#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_GameInstance.h"
#include "01_Game/BTPS_WaveManager.h"
#include "01_Game/BTPS_SpawnManager.h"
#include "02_Characters/BTPS_PlayerController.h"
#include "04_Items/A_Equipment/Weapon/BTPS_WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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

	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerChar)
	{
        
		TArray<AActor*> AttachedActors;
		PlayerChar->GetAttachedActors(AttachedActors);
        
		for (AActor* Actor : AttachedActors)
		{
			ABTPS_WeaponBase* CurrentWeapon = Cast<ABTPS_WeaponBase>(Actor);
			if (CurrentWeapon)
			{
				int32 AmmoToGive = 5; 
				CurrentWeapon->AddReserveAmmo(AmmoToGive);
				break;
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Monster Killed! (%d / %d)"), KilledMonsterCount, SpawnMonsterCount);
	OnMissionScoreChanged.Broadcast(KilledMonsterCount, SpawnMonsterCount);
	
	AddScore(ScoreReward);

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

void ABTPS_GameState::StartLevel()
{
	
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
	
	FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

}

void ABTPS_GameState::EndLevel()
{
	AddScore(LevelScore);
	CurrentLevelIndex++;

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameClear();
		return;
	}

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