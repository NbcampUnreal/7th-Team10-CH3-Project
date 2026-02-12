#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BTPS_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class BASICTPS_API ABTPS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	ABTPS_PlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ToggleCameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ToggleMenuAction;  // Tab 키 메뉴 액션

	// Menu HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* PauseMenuWidgetInstance;
	
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	// HUD 표시
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowPauseMenu();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void HidePauseMenu();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void TogglePauseMenu();
	// 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	
	virtual void BeginPlay() override;

private:
	bool bIsGamePaused = false;
	virtual void SetupInputComponent() override;

};
