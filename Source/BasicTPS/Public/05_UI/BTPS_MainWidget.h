#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTPS_MainWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class UOverlay;
class UBorder;
class UBTPS_StatComponent;
class UBTPS_ShootingMachineComponent;
class UBTPS_GrenadeSlotWidget;

UCLASS()
class BASICTPS_API UBTPS_MainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindStatComp(UBTPS_StatComponent* StatComp);

	void BindShootingComp(UBTPS_ShootingMachineComponent* ShootingMachineComp);
	
	void TickReadTime();

	void ShowWeaponUI();

	UFUNCTION()
	void UpdateCrosshairColor(bool bIsAimingAtEnemy);

	UFUNCTION()
	void ShowKillMarker();

	UFUNCTION()
	void HideKillMarker();

	// UFUNCTION()
	// void SetWeaponImage(UTexture2D* NewTexture);
	
	UBTPS_GrenadeSlotWidget* GetGrenadeSlot() const { return GrenadeSlot; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void UpdateHP(float CurrentHP, float MaxHP);
	
	UFUNCTION()
	void UpdateStamina(float CurrentStamina, float MaxStamina);

	UFUNCTION()
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo, int32 ReserveAmmo);
	
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> WeaponInfoBox;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Timer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Crosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_KillMarker;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBTPS_GrenadeSlotWidget> GrenadeSlot;
	
protected:
	FTimerHandle RetryBindHandle;
	FTimerHandle KillMarkerTimerHandle;
	
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<UImage> WeaponImage;
};
