// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BTPS_WaveState.h"
#include "00_Common/BTPS_CoreTypes.h"
#include "BTPS_GameState.generated.h"

// 점수 변경 호출 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
// 킬 카운트 변경 호출 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKillCountChanged, int32, KilledCount, int32, TotalCount);
// 웨이브 변경 호출 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStateChanged, EBTPS_WaveState, NewWaveState);
// 미션 스코어 위젯용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMissionScoreChanged,int32, KillCount, int32, SpawnCount);
// 킬 발생 시 죽은 에너미 이름 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillLogAdded, FText, EnemyName);

class UBTPS_WaveManager;
class UDataTable;
/**
 * 
 */
UCLASS()
class BASICTPS_API ABTPS_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ABTPS_GameState();
	
	virtual void BeginPlay() override;
	
	static ABTPS_GameState* Get(const UObject* WorldContext);

public:
	//variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 LevelScore = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	int32 SpawnMonsterCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	int32 KilledMonsterCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Settings")
	int32 TotalToSpawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 TotalWaveIndex = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	EBTPS_WaveState WaveState = EBTPS_WaveState::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	TObjectPtr<UBTPS_WaveManager> WaveManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KillLog")
	FKillLogData LastKillLogData; 
	
	UPROPERTY(BlueprintAssignable)
	FOnKillLogAdded OnKillLogAdded;
	
	FTimerHandle HUDUpdateTimerHandle;
	
public:
	// Delegate
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnMissionScoreChanged OnMissionScoreChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Monster")
	FOnKillCountChanged OnKillCountChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveStateChanged OnWaveStateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave|Data")
	UDataTable* WaveDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KillLog")
	TArray<FKillLogData> KillLogs;
	
public:
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Monster")
	void OnMonsterKilled(int32 ScoreReward);
	void AddSpawnCount();

	UFUNCTION(BlueprintPure)
	float GetLevelRemainingTime() const;
	UFUNCTION(BlueprintCallable, Category = "KillLog")
	void AddKillLog(FText EnemyName);

	void OnGameOver();
	void OnLevelTimeUp();	
	
	void StartLevel();
	void EndLevel();
	
protected:
	void OnGameClear();

	
};
