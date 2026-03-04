#include "02_Characters/BTPS_PlayerController.h"
#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_GameInstance.h"
#include "05_UI/BTPS_HUD.h"
#include "05_UI/BTPS_MainWidget.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"

ABTPS_PlayerController::ABTPS_PlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  LookAction(nullptr),
	  JumpAction(nullptr),
	  SprintAction(nullptr),
	  AimAction(nullptr),
	  FireAction(nullptr),
	  InteractAction(nullptr),
	  ReloadAction(nullptr),
	  ToggleCameraAction(nullptr),
	  ToggleMenuAction(nullptr),
	  ThrowGrenadeAction(nullptr),


	  MainMenuWidgetClass(nullptr),
	  MainMenuWidgetInstance(nullptr),
	  PauseMenuWidgetClass(nullptr),
	  PauseMenuWidgetInstance(nullptr),
	  GameOverMenuWidgetClass(nullptr),
	  GameOverMenuWidgetInstance(nullptr),
	  GameClearMenuWidgetClass(nullptr),
	  GameClearMenuWidgetInstance(nullptr)
{
}

void ABTPS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("L_MenuLevel"))
	{
		ShowMainMenu(false);
		
		TArray<AActor*> MenuLevelCameras;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), MenuCameraTag, MenuLevelCameras);

		if (MenuLevelCameras.Num() > 0)
		{
			SetViewTarget(MenuLevelCameras[0]); 
		}

		if (ACharacter* MyChar = GetCharacter())
		{
			MyChar->GetCharacterMovement()->StopMovementImmediately();
			MyChar->SetActorHiddenInGame(false);
		}
	}
	else if (CurrentMapName.Contains(TEXT("L_BasicLevel")))
	{
		bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		SetInputMode(InputMode);

		SetPause(false);
	}
	
	if (MenuCameraActor == nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		MenuCameraActor = GetWorld()->SpawnActor<ACameraActor>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
}

void ABTPS_PlayerController::ShowMainMenu(bool bIsRestart)
{
	ClearAllWidgets();

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(
			MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			ButtonText->SetText(bIsRestart ? FText::FromString(TEXT("Restart")) : FText::FromString(TEXT("Start")));
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(
				MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (UBTPS_GameInstance* BTPS_GameInstance = Cast<UBTPS_GameInstance>(
					UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), BTPS_GameInstance->TotalScore)
					));
				}
			}
		}
	}
}

void ABTPS_PlayerController::ShowPauseMenu()
{
	bIsMenuTransitioning = true;
	
    SetIgnoreLookInput(true);
    SetIgnoreMoveInput(true);
    
    if (ACharacter* MyChar = GetCharacter())
    {
       MyChar->GetCharacterMovement()->StopMovementImmediately();

    	UWidgetComponent* MenuWidgetComp = Cast<UWidgetComponent>(MyChar->GetComponentByClass(UWidgetComponent::StaticClass()));
    	if (MenuWidgetComp)
    	{
    		MenuWidgetComp->SetVisibility(true);
          
    		StartOpacity = CurrentMenuOpacity; 
    		TargetOpacity = 1.0f; 
    		FadeStartTime = GetWorld()->GetTimeSeconds();
    		FadeDuration = 0.5f;

    		GetWorldTimerManager().SetTimer(FadeTimerHandle, this, &ABTPS_PlayerController::SmoothFadeStep, 0.01f, true);
    	}

       TArray<UCameraComponent*> CameraComps;
       MyChar->GetComponents<UCameraComponent>(CameraComps);

       for (UCameraComponent* Cam : CameraComps)
       {
       	if (Cam->ComponentHasTag(TEXT("ToggleMenuCam")) || Cam->GetName().Contains(TEXT("ToggleCam")))
       	{
          	if (MenuCameraActor)
          	{
          		MenuCameraActor->SetActorLocationAndRotation(Cam->GetComponentLocation(), Cam->GetComponentRotation());

          		UCameraComponent* SpawnedCamComp = MenuCameraActor->GetCameraComponent();if (SpawnedCamComp)
          		{
          			SpawnedCamComp->SetFieldOfView(Cam->FieldOfView);

          			SpawnedCamComp->PostProcessSettings.bOverride_MotionBlurAmount = true;
          			SpawnedCamComp->PostProcessSettings.MotionBlurAmount = 0.0f;

          			SpawnedCamComp->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
          			SpawnedCamComp->PostProcessSettings.DepthOfFieldFstop = 100.0f;
                     
          			SpawnedCamComp->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
          			SpawnedCamComp->PostProcessSettings.DepthOfFieldSensorWidth = 144.0f;
          		}
          	}
             break; 
          }
       }

       if (MenuCameraActor)
       {
           SetViewTargetWithBlend(MenuCameraActor, 0.5f, VTBlend_Cubic);
       }
    }

    if (ABTPS_HUD* CurrentHUD = Cast<ABTPS_HUD>(GetHUD()))
    {
       if (UBTPS_MainWidget* MainWidget = CurrentHUD->GetMainWidget())
       {
          MainWidget->SetVisibility(ESlateVisibility::Hidden); 
       }
    }
    
    GetWorldTimerManager().SetTimer(CameraBlendTimerHandle, this, &ABTPS_PlayerController::OnShowMenuBlendFinished, 0.6f, false);
}
void ABTPS_PlayerController::OnShowMenuBlendFinished()
{
    bShowMouseCursor = true;
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);
	
	//
	if (ACharacter* MyChar = GetCharacter())
	{
		UWidgetComponent* MenuWidgetComp = Cast<UWidgetComponent>(MyChar->GetComponentByClass(UWidgetComponent::StaticClass()));
		if (MenuWidgetComp)
		{
			MenuWidgetComp->SetVisibility(true);
		}
	}
	
    bIsGamePaused = true;
    SetPause(true);
	
	bIsMenuTransitioning = false;
}
void ABTPS_PlayerController::HidePauseMenu()
{
	bIsMenuTransitioning = true;
	
	bIsGamePaused = false;
	SetPause(false);

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	if (ACharacter* MyChar = GetCharacter())
	{
		UWidgetComponent* MenuWidgetComp = Cast<UWidgetComponent>(MyChar->GetComponentByClass(UWidgetComponent::StaticClass()));
		if (MenuWidgetComp)
		{
			StartOpacity = CurrentMenuOpacity; 
			TargetOpacity = 0.0f; 
			FadeStartTime = GetWorld()->GetTimeSeconds();
			FadeDuration = 0.1f;  

			GetWorldTimerManager().SetTimer(FadeTimerHandle, this, &ABTPS_PlayerController::SmoothFadeStep, 0.01f, true);
		}
       
		SetViewTargetWithBlend(MyChar, 0.1f, VTBlend_Cubic);

		TArray<UCameraComponent*> CameraComps;
		MyChar->GetComponents<UCameraComponent>(CameraComps);

		for (UCameraComponent* Cam : CameraComps)
		{
			if (Cam->ComponentHasTag(TEXT("ToggleMenuCam")))
			{
				Cam->SetActive(false);
			}
			else
			{
				Cam->SetActive(true);
             
				Cam->PostProcessSettings.bOverride_MotionBlurAmount = true;
				Cam->PostProcessSettings.MotionBlurAmount = 0.0f;
			}
		}
	}

	if (ABTPS_HUD* CurrentHUD = Cast<ABTPS_HUD>(GetHUD()))
	{
		if (UBTPS_MainWidget* MainWidget = CurrentHUD->GetMainWidget())
		{
			MainWidget->SetVisibility(ESlateVisibility::Visible); 
		}
	}

	GetWorldTimerManager().SetTimer(CameraBlendTimerHandle, this, &ABTPS_PlayerController::OnHideMenuBlendFinished, 0.1f, false);
}
void ABTPS_PlayerController::OnHideMenuBlendFinished()
{
    SetIgnoreLookInput(false);
    SetIgnoreMoveInput(false);

	bIsMenuTransitioning = false;
}

void ABTPS_PlayerController::TogglePauseMenu()
{
	if (GetWorld()->GetMapName().Contains(TEXT("L_MenuLevel")))
	{
		return;
	}
	
	if (bIsMenuTransitioning)
	{
		return;
	}

	if (bIsGamePaused)
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}

void ABTPS_PlayerController::SmoothFadeStep()
{
	if (ACharacter* MyChar = GetCharacter())
	{
		UWidgetComponent* MenuWidgetComp = Cast<UWidgetComponent>(MyChar->GetComponentByClass(UWidgetComponent::StaticClass()));
		if (MenuWidgetComp)
		{
			float CurrentTime = GetWorld()->GetTimeSeconds();
			float Alpha = (CurrentTime - FadeStartTime) / FadeDuration;
			Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

			CurrentMenuOpacity = FMath::Lerp(StartOpacity, TargetOpacity, Alpha);
            
			MenuWidgetComp->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, CurrentMenuOpacity));

			if (Alpha >= 1.0f)
			{
				GetWorldTimerManager().ClearTimer(FadeTimerHandle);

				if (TargetOpacity <= 0.0f)
				{
					MenuWidgetComp->SetVisibility(false);
				}
			}
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(FadeTimerHandle);
	}
}

void ABTPS_PlayerController::ShowGameOverMenu(bool bIsRestart)
{
	ClearAllWidgets();

	if (GameOverMenuWidgetClass)
	{
		GameOverMenuWidgetInstance = CreateWidget<UUserWidget>(this, GameOverMenuWidgetClass);
		if (GameOverMenuWidgetInstance)
		{
			GameOverMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}

	bIsGamePaused = true;
	SetPause(true);
	
	if (ABTPS_HUD* CurrentHUD = Cast<ABTPS_HUD>(GetHUD()))
	{
		if (UBTPS_MainWidget* MainWidget = CurrentHUD->GetMainWidget())
		{
			MainWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABTPS_PlayerController::ShowGameClearMenu()
{
	ClearAllWidgets();

	if (GameClearMenuWidgetClass)
	{
		GameClearMenuWidgetInstance = CreateWidget<UUserWidget>(this, GameClearMenuWidgetClass);
		if (GameClearMenuWidgetInstance)
		{
			GameClearMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}

	bIsGamePaused = true;
	SetPause(true);
	
	if (ABTPS_HUD* CurrentHUD = Cast<ABTPS_HUD>(GetHUD()))
	{
		if (UBTPS_MainWidget* MainWidget = CurrentHUD->GetMainWidget())
		{
			MainWidget->SetVisibility(ESlateVisibility::Hidden); 
		}
	}
}


void ABTPS_PlayerController::StartGame()
{
	if (UBTPS_GameInstance* BTPS_GameInstance = Cast<UBTPS_GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		BTPS_GameInstance->CurrentLevelIndex = 0;
		BTPS_GameInstance->TotalScore = 0;
	}


	ClearAllWidgets();

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	SetPause(false);

	if (GetCharacter())
	{
		StartRotation = GetCharacter()->GetActorRotation();

		TargetRotation = StartRotation + FRotator(0.0f, 179.9f, 0.0f);

		SequenceStartTime = GetWorld()->GetTimeSeconds();
		RotationDuration = 0.5f;

		GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ABTPS_PlayerController::SmoothRotateStep, 0.01f,
		                                true);
	}
}

void ABTPS_PlayerController::LoadGameplayLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("L_BasicLevel"));
}

void ABTPS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("PlayerController SetupInputComponent called"));

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ToggleMenuAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("MenuAction binding success!"));
			EnhancedInput->BindAction(ToggleMenuAction, ETriggerEvent::Triggered, this,
			                          &ABTPS_PlayerController::TogglePauseMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MenuAction is NULL!"));
		}

		if (SkipLevelAction)
		{
			EnhancedInput->BindAction(SkipLevelAction, ETriggerEvent::Started, this,
			                          &ABTPS_PlayerController::OnSkipLevel);
		}
	}
}

void ABTPS_PlayerController::OnSkipLevel()
{
	if (ABTPS_GameState* BTPSGameState = Cast<ABTPS_GameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		BTPSGameState->EndLevel();

		UE_LOG(LogTemp, Warning, TEXT("Cheat Activated: Skipping to Next Level!"));
	}
}

void ABTPS_PlayerController::SmoothRotateStep()
{
	if (!GetCharacter())
	{
		GetWorldTimerManager().ClearTimer(RotationTimerHandle);
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float Alpha = (CurrentTime - SequenceStartTime) / RotationDuration;

	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

	FRotator NewRot = FMath::InterpEaseInOut(StartRotation, TargetRotation, Alpha, 2.0f);

	GetCharacter()->SetActorRotation(NewRot);
	SetControlRotation(NewRot);

	if (Alpha >= 1.0f)
	{
		GetCharacter()->SetActorRotation(TargetRotation);
		SetControlRotation(TargetRotation);

		GetWorldTimerManager().ClearTimer(RotationTimerHandle);

		SetViewTargetWithBlend(GetCharacter(), 1.5f, VTBlend_Cubic, 0.0f, true);
	}
}

void ABTPS_PlayerController::ClearAllWidgets()
{
	TObjectPtr<UUserWidget> WidgetsToClear[] = {
		
		MainMenuWidgetInstance,
		PauseMenuWidgetInstance,
		GameOverMenuWidgetInstance,
		GameClearMenuWidgetInstance
	};

	for (TObjectPtr<UUserWidget>& Widget : WidgetsToClear)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
			Widget = nullptr;
		}
	}
}
