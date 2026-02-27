#include "02_Characters/BTPS_EnemyCharacterBase.h"
#include "01_Game/BTPS_GameState.h"
#include "BrainComponent.h"
#include "03_Components/BTPS_StatComponent.h"
#include "05_UI/BTPS_EnemyHealthBarWidget.h"
#include "06_Ai/BTPS_AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


ABTPS_EnemyCharacterBase::ABTPS_EnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	TeamID = FGenericTeamId(1);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);

	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(120.0f, 15.0f));
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	
	VisionConeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("VisionConeDecal"));
	VisionConeDecal->SetupAttachment(RootComponent);
	VisionConeDecal->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	VisionConeDecal->DecalSize = FVector(200.0f,SightRadius, SightRadius);
}

void ABTPS_EnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetComponent)
	{
		UBTPS_EnemyHealthBarWidget* HealthBarWidget = Cast<UBTPS_EnemyHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject());

		if (HealthBarWidget && StatComp)
		{
			HealthBarWidget->BindStatComp(StatComp);
		}
	}
}

void ABTPS_EnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (HealthBarWidgetComponent)
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			// 너무 멀리 있는 적은 계산할 필요가 없다면 거리 체크 후 연산
			// float Distance = FVector::Dist(GetActorLocation(), CameraManager->GetCameraLocation());
			// if (Distance > 3000.0f) return;

			FVector CameraLocation = CameraManager->GetCameraLocation();
			FVector WidgetLocation = HealthBarWidgetComponent->GetComponentLocation();

			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
			HealthBarWidgetComponent->SetWorldRotation(LookAtRotation);
		}
	}
	
	if (VisionMatInstance)
	{
		float CurrentOpacity = 0.0f;

		if (HasTarget())
		{
			CurrentOpacity = 1.0f;
		}
		else
		{
			ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (PlayerChar)
			{
				float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerChar->GetActorLocation());
                
				CurrentOpacity = FMath::GetMappedRangeValueClamped(
					FVector2D(SightRadius + DecalFadeMinDistance, LoseSightRadius + DecalFadeMaxDistance), 
					FVector2D(1.0f, 0.0f), 
					DistanceToPlayer
				);
			}
		}
		VisionMatInstance->SetScalarParameterValue(TEXT("FadeOpacity"), CurrentOpacity);
	}
}


void ABTPS_EnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABTPS_EnemyCharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (VisionConeDecal)
	{
		VisionConeDecal->DecalSize = FVector(200.0f, SightRadius, SightRadius);
		
		VisionMatInstance = Cast<UMaterialInstanceDynamic>(VisionConeDecal->GetDecalMaterial());
        
		if (!VisionMatInstance)
		{
			VisionMatInstance = VisionConeDecal->CreateDynamicMaterialInstance();
		}

		if (VisionMatInstance)
		{
			float CosValue = FMath::Cos(FMath::DegreesToRadians(PeripheralVisionAngle));
			VisionMatInstance->SetScalarParameterValue(TEXT("CosHalfAngle"), CosValue);
			VisionMatInstance->SetVectorParameterValue(TEXT("VisionColor"), NormalVisionColor);
		}
	}
}

void ABTPS_EnemyCharacterBase::SetTarget(AActor* NewTarget)
{
	if (!IsValid(NewTarget)) return;

	if (NewTarget == this) return;

	CurrentTarget = NewTarget;

	if (VisionMatInstance)
	{
		VisionMatInstance->SetVectorParameterValue(TEXT("VisionColor"), AlertVisionColor);
	}
}

AActor* ABTPS_EnemyCharacterBase::GetTarget() const
{
	return CurrentTarget.Get();
}

bool ABTPS_EnemyCharacterBase::HasTarget() const
{
	return CurrentTarget.IsValid();
}

void ABTPS_EnemyCharacterBase::ClearTarget()
{
	CurrentTarget = nullptr;
	
	if (VisionMatInstance)
	{
		VisionMatInstance->SetVectorParameterValue(TEXT("VisionColor"), NormalVisionColor);
	}
}

void ABTPS_EnemyCharacterBase::OnDeath()
{
	Super::OnDeath();

	ABTPS_AIController* AIController = Cast<ABTPS_AIController>(GetController());
	if (AIController && AIController->GetBrainComponent())
	{
		AIController->StopMovement();
		AIController->GetBrainComponent()->StopLogic("Dead");
	}
	
	// 에너미 사망시 물리 시뮬
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetCollisionProfileName(TEXT("PhysicsActor"));
        
		Capsule->BodyInstance.bLockXRotation = false;
		Capsule->BodyInstance.bLockYRotation = false;
        
		Capsule->SetEnableGravity(true);
		Capsule->SetSimulatePhysics(true);
        
		FVector PushDirection = GetActorForwardVector() * -1.0f; // 캐릭터의 뒤쪽 방향
		Capsule->AddImpulse(PushDirection * 1500.0f, NAME_None, true); // 힘(1500)을 가함
	}
	
	if (ABTPS_GameState* GS = GetWorld()->GetGameState<ABTPS_GameState>())
	{
		GS->OnMonsterKilled(10);
	}
	
	if (VisionConeDecal)
	{
		VisionConeDecal->SetVisibility(false);
	}
	
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}
	
	SetLifeSpan(3.0f);
}
