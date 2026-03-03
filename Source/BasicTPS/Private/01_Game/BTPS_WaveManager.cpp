#include "01_Game/BTPS_WaveManager.h"
#include "01_Game/BTPS_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "05_UI/BTPS_HUD.h"

void UBTPS_WaveManager::Init(ABTPS_GameState* InGameState)
{
	GameStateRef = InGameState;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GameStateRef->GetWorld(), ABTPS_SpawnManager::StaticClass(), FoundVolumes);

	for (AActor* Actor : FoundVolumes)
	{
		if (ABTPS_SpawnManager* Spawner = Cast<ABTPS_SpawnManager>(Actor))
		{
			SpawnManagers.Add(Spawner);
		}
	}

	Waves.Empty();

	if (GameStateRef && GameStateRef->WaveDataTable)
	{
		FString ContextString = TEXT("WaveData Context");
		TArray<FBTPS_WaveData*> AllWaveRows;

		GameStateRef->WaveDataTable->GetAllRows<FBTPS_WaveData>(ContextString, AllWaveRows);

		for (FBTPS_WaveData* Row : AllWaveRows)
		{
			if (Row)
			{
				Waves.Add(*Row);
			}
		}
	}
}

void UBTPS_WaveManager::StartWave(int32 WaveIndex)
{
	CurrentWaveIndex = WaveIndex;
	CurrentState = EBTPS_WaveState::Begin;

	if (WaveIndex >= Waves.Num())
		return;

	if (!GameStateRef || !GameStateRef->GetWorld()) return;

	UWorld* World = GameStateRef->GetWorld();

	FTimerHandle UIDelayTimer;
	World->GetTimerManager().SetTimer(UIDelayTimer, FTimerDelegate::CreateWeakLambda(this, [this, WaveIndex, World]()
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				if (ABTPS_HUD* HUD = Cast<ABTPS_HUD>(PC->GetHUD()))
				{
					HUD->ShowWaveAlert(WaveIndex + 1);
				}
			}
		}), 0.5f, false);


	FTimerHandle WaveStartDelayTimer;
	World->GetTimerManager().SetTimer(WaveStartDelayTimer, FTimerDelegate::CreateWeakLambda(this, [this, WaveIndex]()
		{
			OnWaveChanged.Broadcast(CurrentWaveIndex + 1, Waves.Num());

			const FBTPS_WaveData& Wave = Waves[WaveIndex];

			AliveEnemyCount += Wave.EnemyCount;
			CurrentState = EBTPS_WaveState::Progress;
			SpawnedEnemyCount = 0;

			if (SpawnManagers.Num() > 0 && SpawnManagers[0]->GetWorld())
			{
				SpawnManagers[0]->GetWorld()->GetTimerManager().SetTimer(
					SpawnTimerHandle,
					this,
					&UBTPS_WaveManager::SpawnEnemyByTimer,
					Wave.SpawnInterval,
					true
				);
			}

			if (GameStateRef && GameStateRef->GetWorld())
			{
				GameStateRef->GetWorld()->GetTimerManager().SetTimer(
					WaveTimerHandle,
					this,
					&UBTPS_WaveManager::EndWave,
					Wave.WaveDuration,
					false
				);
			}
		}), 3.5f, false);
}

void UBTPS_WaveManager::EndWave()
{
	CurrentState = EBTPS_WaveState::End;

	int32 NextWaveIndex = CurrentWaveIndex + 1;

	if (NextWaveIndex < Waves.Num())
	{
		StartWave(NextWaveIndex);
	}
	else
	{
		if (GameStateRef)
		{
			GameStateRef->EndLevel();
		}
	}
}

void UBTPS_WaveManager::OnEnemyDead()
{	//TODO_CSH 몬스터 사망 함수가 이 함수를 호출해 줘야 정상적으로 웨이브 구현이 가능
	AliveEnemyCount--;

	if (CurrentState != EBTPS_WaveState::Progress)
	{
		return;
	}

	if (AliveEnemyCount <= 0)
	{
		if (UWorld* World = GameStateRef->GetWorld())
		{
			World->GetTimerManager().ClearTimer(WaveTimerHandle);
			World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		}

		EndWave();
	}
}

void UBTPS_WaveManager::SpawnEnemyByTimer()
{
	int32 RandomIndex = FMath::RandRange(0, SpawnManagers.Num() - 1);
	SpawnManagers[RandomIndex]->SpawnRandomEnemy();

	SpawnedEnemyCount++;
	
	GameStateRef->AddSpawnCount();

	if (SpawnedEnemyCount >= Waves[CurrentWaveIndex].EnemyCount)
	{
		if (UWorld* World = SpawnManagers[0]->GetWorld())
		{
			World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		}
	}
}

float UBTPS_WaveManager::GetWaveRemainingTime() const
{
	if (GameStateRef && GameStateRef->GetWorld())
	{
		return GameStateRef->GetWorld()->GetTimerManager().GetTimerRemaining(WaveTimerHandle);
	}
	return 0.0f;
}