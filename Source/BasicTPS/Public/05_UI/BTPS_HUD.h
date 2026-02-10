// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BTPS_HUD.generated.h"

class UBTPS_MainWidget;

UCLASS()
class BASICTPS_API ABTPS_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBTPS_MainWidget> MainWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UBTPS_MainWidget> MainWidget;

public:
	virtual void BeginPlay() override;
	UBTPS_MainWidget* GetMainWidget() const {return MainWidget;}
};
