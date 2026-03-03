#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BTPS_HUD.generated.h"

class UBTPS_MainWidget;
class UBTPS_WaveAlertWidget;

UCLASS()
class BASICTPS_API ABTPS_HUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBTPS_MainWidget> MainWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBTPS_WaveAlertWidget> WaveAlertWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UBTPS_MainWidget> MainWidget;

	UPROPERTY()
	TObjectPtr<UBTPS_WaveAlertWidget> WaveAlertWidget;

	FTimerHandle WaveAlertTimerHandle;

public:
	virtual void BeginPlay() override;
	UBTPS_MainWidget* GetMainWidget() const {return MainWidget;}

	void HideMainUI();
	void ShowWaveAlert(int32 WaveLevel);
	void RestoreMainUI();
};
