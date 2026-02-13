#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "01_Game/BTPS_SpawnManager.h"
#include "01_Game/BTPS_WaveState.h"
#include "BTPS_WaveManager.generated.h"

USTRUCT(BlueprintType)
struct FBTPS_WaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 EnemyCount = 5;
	UPROPERTY(EditAnywhere)
	float SpawnInterval = 0.5f;
};


UCLASS()
class BASICTPS_API UBTPS_WaveManager : public UObject
{
	GENERATED_BODY()
	
public:

	void StartWave(int32 WaveIndex);
	void OnEnemyDead();

protected:
	int32 AliveEnemyCount;
	int32 CurrentWaveIndex;
	EBTPS_WaveState CurrentState;

	UPROPERTY()
	TArray<ABTPS_SpawnManager*> SpawnManagers;
	UPROPERTY()
	TArray<FBTPS_WaveData> Waves;
};
