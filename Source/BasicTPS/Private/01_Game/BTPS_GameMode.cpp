// Fill out your copyright notice in the Description page of Project Settings.

#include "01_Game/BTPS_GameMode.h"
#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_GameInstance.h"

#include "02_Characters/BTPS_PlayerCharacter.h"
#include "02_Characters/BTPS_PlayerController.h"


ABTPS_GameMode::ABTPS_GameMode()
{
	PlayerControllerClass = ABTPS_PlayerController::StaticClass();
	
	DefaultPawnClass = ABTPS_PlayerCharacter::StaticClass();
	
	GameStateClass = ABTPS_GameState::StaticClass();
}

ABTPS_GameMode* ABTPS_GameMode::Get(const UObject* WorldContext)
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
	
	return World->GetAuthGameMode<ABTPS_GameMode>();
}


void ABTPS_GameMode::StartPlay()
{
	Super::StartPlay();
}

void ABTPS_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABTPS_GameMode::StartLevel()
{
	UBTPS_GameInstance* BTPS_GameInstance = UBTPS_GameInstance::Get(this);
	if (BTPS_GameInstance == nullptr || BTPS_GameInstance->IsMenuLevel())
	{
		return;
	}
}

void ABTPS_GameMode::EndLevel(bool bWin)
{
}

void ABTPS_GameMode::GameOver()
{
}

void ABTPS_GameMode::KilledMonsterCount()
{
}

void ABTPS_GameMode::SpawnMonstersForWave()
{
}

void ABTPS_GameMode::OnLevelTimeUp()
{
}


