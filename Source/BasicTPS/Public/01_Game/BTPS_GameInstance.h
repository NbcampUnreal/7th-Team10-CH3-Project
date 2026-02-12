// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BTPS_GameInstance.generated.h"



/**
 * 
 */
UCLASS()
class BASICTPS_API UBTPS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBTPS_GameInstance();
	
	static UBTPS_GameInstance* Get(const UObject* WorldContext);
	
public:
	// getters
	FORCEINLINE int32 GetTotalScore() const {return TotalScore;}
	FORCEINLINE int32 GetCurrentLevelIndex() const {return CurrentLevelIndex;}
	int32 GetMaxLevels() const;
	bool IsLastLevel() const;
	bool IsMenuLevel() const;
	
public:
	// funtuions
	void AddToScore(int32 Amount);
	void SetCurrentLevelIndex(int32 Index);
	void RestartGame();
	
	void OpenMainMenuLevel();
	void OpenGameLevel(int32 LevelIndex);
	void OpenNextLevel();
	
public:
	// variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="GmaeData", meta = (AllowPrivateAccess = "true"))
	int32 TotalScore = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="GmaeData", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevelIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> MenuLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataTable> ItemDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataTable> WaveDataTable;
	
};
