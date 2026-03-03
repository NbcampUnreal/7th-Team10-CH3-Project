#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "01_Game/BTPS_SpawnManager.h"
#include "01_Game/BTPS_WaveState.h"
#include "Engine/DataTable.h"
#include "BTPS_WaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveChanged, int32, CurrentWave, int32, TotalWaves);

class ABTPS_GameState;

USTRUCT(BlueprintType)
struct FBTPS_WaveData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyCount = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDuration = 30.0f;
};


UCLASS()
class BASICTPS_API UBTPS_WaveManager : public UObject
{
	GENERATED_BODY()
	
public:
	//delegate
	UPROPERTY(BlueprintAssignable)
	FOnWaveChanged OnWaveChanged;
	
public:
	void Init(ABTPS_GameState* InGameState);
	void StartWave(int32 WaveIndex);
	void EndWave();
	void OnEnemyDead();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	float GetWaveRemainingTime() const;
	
	UFUNCTION(BlueprintPure, Category = "WaveInfo")
	int32 GetCurrentWaveIndex() const { return CurrentWaveIndex; }

	UFUNCTION(BlueprintPure, Category = "WaveInfo")
	int32 GetTotalWaves() const { return Waves.Num(); }

protected:
	int32 AliveEnemyCount;
	int32 CurrentWaveIndex;
	EBTPS_WaveState CurrentState;

	UPROPERTY()
	TArray<ABTPS_SpawnManager*> SpawnManagers;
	UPROPERTY()
	TArray<FBTPS_WaveData> Waves;

	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveTimerHandle;

	int32 SpawnedEnemyCount;

	UFUNCTION()
	void SpawnEnemyByTimer();

private:
	ABTPS_GameState* GameStateRef;
};
