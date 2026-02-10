// Fill out your copyright notice in the Description page of Project Settings.


#include "01_Game/BTPS_GameState.h"

ABTPS_GameState::ABTPS_GameState()
{
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
