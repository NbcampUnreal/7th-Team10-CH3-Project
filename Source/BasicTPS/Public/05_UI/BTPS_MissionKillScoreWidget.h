// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "BTPS_MissionKillScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASICTPS_API UBTPS_MissionKillScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_MissionScore;
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateMissionScore(int32 killMonster, int32 SpawnMonster);
};
