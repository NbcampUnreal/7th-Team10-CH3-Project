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
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_CurrentWave;
};
