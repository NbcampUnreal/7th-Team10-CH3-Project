// Fill out your copyright notice in the Description page of Project Settings.


#include "01_Game/BTPS_GameInstance.h"

#include "Kismet/GameplayStatics.h"

UBTPS_GameInstance::UBTPS_GameInstance()
{
}

UBTPS_GameInstance* UBTPS_GameInstance::Get(const UObject* WorldContext)
{
	return Cast<UBTPS_GameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
}

int32 UBTPS_GameInstance::GetMaxLevels() const
{
	return 0;
}

bool UBTPS_GameInstance::IsLastLevel() const
{
	return CurrentLevelIndex >= GetMaxLevels() -1;
	
}

bool UBTPS_GameInstance::IsMenuLevel() const
{
	UWorld* World = GetWorld();
	if (World == nullptr || MenuLevel.IsNull())
	{
		return false;
	}
	
	FString CurrentMapName = World->GetMapName();
	CurrentMapName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	FString MenuLevelName = MenuLevel.GetAssetName();
	return CurrentMapName.Contains(MenuLevelName);
}

void UBTPS_GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}