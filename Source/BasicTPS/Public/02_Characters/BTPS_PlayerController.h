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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ToggleCameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ToggleMenuAction;  // Tab 키 메뉴 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkipLevelAction; // pgdown 레벨스킵 치트키
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ThrowGrenadeAction;
	
	// Menu HUD
	
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* PauseMenuWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> GameOverMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* GameOverMenuWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> GameClearMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	TObjectPtr<UUserWidget> GameClearMenuWidgetInstance;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	FName MenuCameraTag = TEXT("MenuCam");
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowPauseMenu();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void HidePauseMenu();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void TogglePauseMenu();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameOverMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameClearMenu();
	
	// 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void LoadGameplayLevel();
	
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SmoothRotateStep();
	
	bool bIsGamePaused = false;
	virtual void SetupInputComponent() override;
	void OnSkipLevel();
	void ClearAllWidgets();
	
	// 시작메뉴 캐릭터 움직임 함수
	FTimerHandle RotationTimerHandle;
	FRotator StartRotation;  
	FRotator TargetRotation; 
	float SequenceStartTime;  
	float RotationDuration = 0.5f;   
	
	// 메뉴 카메라 블렌딩 함수
	FTimerHandle CameraBlendTimerHandle;
	UPROPERTY()
	ACameraActor* MenuCameraActor;
	void OnShowMenuBlendFinished();
	void OnHideMenuBlendFinished();
	
	FTimerHandle FadeTimerHandle;
	float FadeStartTime;
	float FadeDuration;
	float StartOpacity;
	float TargetOpacity;
	float CurrentMenuOpacity = 0.0f;
	void SmoothFadeStep();
	
	bool bIsMenuTransitioning = false;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	FName ToggleCameraTag = TEXT("ToggleMenuCam");
	
	

};
