// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTPS_WaveAlertWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class BASICTPS_API UBTPS_WaveAlertWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayWaveStartAlert(int32 WaveLevel);

	float GetWaveAlertDuration() const;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveStartText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> WaveStartAnim;
};
