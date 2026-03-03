// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "BTPS_WaveWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASICTPS_API UBTPS_WaveWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void UpdateWaveText(int32 CurrentWave, int32 TotalWaves);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_CurrentWave;
};
