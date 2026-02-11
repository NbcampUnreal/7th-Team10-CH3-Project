// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BTPS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class BASICTPS_API ABTPS_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABTPS_GameMode();
	
	//Getter
	static ABTPS_GameMode* Get(const UObject* WorldContext);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	//functions
	void StartLevel();
	void EndLevel(bool bWin);
	void GameOver();
	void KilledMonsterCount();
	void SpawnMonstersForWave();
	
private:
	void OnLevelTimeUp();
	
	
};
