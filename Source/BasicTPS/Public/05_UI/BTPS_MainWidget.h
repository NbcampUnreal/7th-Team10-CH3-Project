// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTPS_MainWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class UBTPS_StatComponent;
class UBTPS_ShootingMachineComponent;

UCLASS()
class BASICTPS_API UBTPS_MainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindStatComp(UBTPS_StatComponent* StatComp);

	void BindShootingComp(UBTPS_ShootingMachineComponent* ShootingMachineComp);
	
	void TickReadTime();
	// UFUNCTION()
	// void SetWeaponImage(UTexture2D* NewTexture);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void UpdateHP(float CurrentHP, float MaxHP);
	
	UFUNCTION()
	void UpdateStamina(float CurrentStamina, float MaxStamina);

	UFUNCTION()
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);
	
	UFUNCTION()
	void UpdateLevelDuration();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HPText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StaminaText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_AmmoInfo;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Timer;
	
protected:
	FTimerHandle RetryBindHandle;
	
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<UImage> WeaponImage;
};
