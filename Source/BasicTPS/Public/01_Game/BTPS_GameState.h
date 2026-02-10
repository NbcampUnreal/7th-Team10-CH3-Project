// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BTPS_WaveState.h"
#include "BTPS_GameState.generated.h"

// 점수 변경 호출 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
// 킬 카운트 변경 호출 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKillCountChanged, int32, KilledCount, int32, TotalCount);
// 웨이브 변경 호출 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStateChanged, EBTPS_WaveState, NewWaveState);




/**
 * 
 */
UCLASS()
class BASICTPS_API ABTPS_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ABTPS_GameState();
	
	static ABTPS_GameState* Get(const UObject* WorldContext);
	
public:
	//variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 LevelScore = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	int32 SpawnMonsterCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	int32 KilledMonsterCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 TotalWaveIndex = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	EBTPS_WaveState WaveState = EBTPS_WaveState::None;
	
public:
	// Delegate
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Monster")
	FOnKillCountChanged OnKillCountChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveStateChanged OnWaveStateChanged;
	
	
	
	
};
