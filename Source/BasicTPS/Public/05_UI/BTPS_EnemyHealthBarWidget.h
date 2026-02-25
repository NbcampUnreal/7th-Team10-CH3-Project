// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTPS_EnemyHealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class UBTPS_EnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void BindStatComp(class UBTPS_StatComponent* StatComp);

	UFUNCTION()
	void UpdateHealthBar(float CurrentHP, float MaxHP);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPProgressBar;
};
