#include "01_Game/BTPS_WaveManager.h"

void UBTPS_WaveManager::StartWave(int32 WaveIndex)
{
	CurrentWaveIndex = WaveIndex;
	CurrentState = EBTPS_WaveState::Begin;

	const FBTPS_WaveData& Wave = Waves[WaveIndex];

	AliveEnemyCount = Wave.EnemyCount;
	CurrentState = EBTPS_WaveState::Progress;

	if (SpawnManagers.Num() == 0) return;

	for (int32 i = 0; i < Wave.EnemyCount; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnManagers.Num() - 1);
		SpawnManagers[RandomIndex]->SpawnRandomEnemy();
	}
}

void UBTPS_WaveManager::OnEnemyDead()
{	//TODO_CSH 몬스터 사망 함수가 이 함수를 호출해 줘야 정상적으로 웨이브 구현이 가능
	AliveEnemyCount--;

	if (AliveEnemyCount <= 0)
	{
		CurrentState = EBTPS_WaveState::End;

		StartWave(CurrentWaveIndex + 1);
	}
}
