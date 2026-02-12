#include "02_Characters/BTPS_PlayerController.h"
#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ABTPS_PlayerController::ABTPS_PlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),	
	  LookAction(nullptr),
	  JumpAction(nullptr),
	  SprintAction(nullptr),
	  AimAction(nullptr),
	  FireAction(nullptr),
	  InteractAction(nullptr),
	  ToggleCameraAction(nullptr),
	  ToggleMenuAction(nullptr),
	  HUDWidgetClass(nullptr), 
	  HUDWidgetInstance(nullptr),
	  MainMenuWidgetClass(nullptr),
	  MainMenuWidgetInstance(nullptr),
	  PauseMenuWidgetClass(nullptr),
	  PauseMenuWidgetInstance(nullptr), 
	  bIsGamePaused(false) 
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
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("L_MenuLevel"))
	{
		ShowMainMenu(false);
	}
	else
	{
		ShowGameHUD();
	}
}


UUserWidget* ABTPS_PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ABTPS_PlayerController::ShowGameHUD()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (PauseMenuWidgetInstance)
	{
		PauseMenuWidgetInstance->RemoveFromParent();
		PauseMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		ABTPS_GameState* BTPS_GameState = GetWorld() ? GetWorld()->GetGameState<ABTPS_GameState>() : nullptr;
		if (BTPS_GameState)
		{
			BTPS_GameState->UpdateHUD();
		}
	}
}

void ABTPS_PlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			//UE_LOG(LogTemp, Warning, TEXT("SceneSwitchInputMode"))

			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(
			MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
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
	if (PauseMenuWidgetInstance)
	{
		PauseMenuWidgetInstance->RemoveFromParent();
		PauseMenuWidgetInstance = nullptr;
	}

	if (PauseMenuWidgetClass)
	{
		PauseMenuWidgetInstance = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
		if (PauseMenuWidgetInstance)
		{
			PauseMenuWidgetInstance->AddToViewport(10);

			bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
		}
	}

	bIsGamePaused = true;
	SetPause(true);
}

void ABTPS_PlayerController::HidePauseMenu()
{
	if (PauseMenuWidgetInstance)
	{
		PauseMenuWidgetInstance->RemoveFromParent();
		PauseMenuWidgetInstance = nullptr;
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	bIsGamePaused = false;
	SetPause(false);
}

void ABTPS_PlayerController::TogglePauseMenu()
{
	if (bIsGamePaused)
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}

void ABTPS_PlayerController::StartGame()
{
	if (UBTPS_GameInstance* BTPS_GameInstance = Cast<UBTPS_GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		BTPS_GameInstance->CurrentLevelIndex = 0;
		BTPS_GameInstance->TotalScore = 0;
	}

	// 레벨 전환 전에 InputMode를 Game 모드로 변경
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	UGameplayStatics::OpenLevel(GetWorld(), FName("L_BasicLevel"));
	SetPause(false);
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
	}
}
