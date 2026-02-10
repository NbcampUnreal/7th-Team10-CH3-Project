// Fill out your copyright notice in the Description page of Project Settings.


#include "05_UI/BTPS_HUD.h"
#include "05_UI/BTPS_MainWidget.h"

void ABTPS_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UBTPS_MainWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}
}