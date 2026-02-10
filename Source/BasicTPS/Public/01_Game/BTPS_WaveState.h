// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTPS_WaveState.generated.h"

UENUM(BlueprintType)
enum class EBTPS_WaveState : uint8
{
	None         UMETA(DisplayName = "None"),
	Begin        UMETA(DisplayName = "Begin"),
	Progress     UMETA(DisplayName = "Progress"),
	End          UMETA(DisplayName = "End"),
	Intermission UMETA(DisplayName = "Intermission")
};
