#include "02_Characters/BTPS_PlayerController.h"
#include "01_Game/BTPS_GameState.h"
#include "01_Game/BTPS_GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	ThrowGrenadeAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	PauseMenuWidgetClass(nullptr),
	PauseMenuWidgetInstance(nullptr),
	GameOverMenuWidgetClass(nullptr),
	GameOverMenuWidgetInstance(nullptr)
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
		TArray<AActor*> FoundCameras;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), MenuCameraTag, FoundCameras);

		if (FoundCameras.Num() > 0)
		{
			SetViewTarget(FoundCameras[0]); // 시작 시점은 메뉴 카메라
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

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	SetPause(false);
	bIsGamePaused = false;

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

void ABTPS_PlayerController::ShowGameOverMenu(bool bIsRestart)
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

	
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
	SetPause(false);

	if (GetCharacter())
	{
		StartRotation = GetCharacter()->GetActorRotation();

		TargetRotation = StartRotation + FRotator(0.0f, 179.9f, 0.0f);

		SequenceStartTime = GetWorld()->GetTimeSeconds();
		RotationDuration = 0.5f;

		GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ABTPS_PlayerController::SmoothRotateStep, 0.01f, true);
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
			EnhancedInput->BindAction(SkipLevelAction, ETriggerEvent::Started, this, &ABTPS_PlayerController::OnSkipLevel);
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