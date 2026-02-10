// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTPS_MainWidget.generated.h"

class UProgressBar;
class UBTPS_StatComponent;

UCLASS()
class BASICTPS_API UBTPS_MainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindStatComp(UBTPS_StatComponent* StatComp);
	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void UpdateHP(float CurrentHP, float MaxHP);
	
	UFUNCTION()
	void UpdateStamina(float CurrentStamina, float MaxStamina);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
};
