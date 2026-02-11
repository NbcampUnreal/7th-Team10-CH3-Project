#include "03_Components/BTPS_ShootingMachineComponent.h"
#include "EnhancedInputComponent.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"
#include "03_Components/BTPS_CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "04_Items/A_Equipment/Weapon/BTPS_WeaponBase.h"
// #include "CollisionAndTrace5_6Character.h" 팀플 camera component구현한 헤더 포함해야함, camera component는 public으로 해야함.


UBTPS_ShootingMachineComponent::UBTPS_ShootingMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UBTPS_ShootingMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ACharacter>(GetOwner());
	
	if (GetOwner())
	{
		CombatComp = GetOwner()->FindComponentByClass<UBTPS_CombatComponent>();
	}
}


void UBTPS_ShootingMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

/* action binding할때 여기 참조해서 같이 바인딩.
void UBTPS_ShootingMachineComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		if (AimAction)
		{
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &UBTPS_ShootingMachineComponent::AimStarted);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &UBTPS_ShootingMachineComponent::AimCompleted);
		}

		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UBTPS_ShootingMachineComponent::Fire);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UBTPS_ShootingMachineComponent::Interact);
		}
	}
}
*/


void UBTPS_ShootingMachineComponent::AimStarted(const FInputActionValue& Value)
{
	DoAimStart();
}

void UBTPS_ShootingMachineComponent::AimCompleted(const FInputActionValue& Value)
{
	DoAimEnd();
}

void UBTPS_ShootingMachineComponent::Fire(const FInputActionValue& Value)
{
	DoFire();
}

void UBTPS_ShootingMachineComponent::Interact(const FInputActionValue& Value)
{
	DoInteract();
}

void UBTPS_ShootingMachineComponent::ToggleCamera(const FInputActionValue& Value)
{
	DoToggleCamera();
}


//move에 추가 	if (bIsAiming) return;

void UBTPS_ShootingMachineComponent::DoAimStart()
{
	bIsAiming = true;

	if (PlayerCharacter && AimMontage)
	{
		PlayerCharacter->PlayAnimMontage(AimMontage);
	}
}

void UBTPS_ShootingMachineComponent::DoAimEnd()
{
	bIsAiming = false;

	if (PlayerCharacter && AimMontage)
	{
		PlayerCharacter->StopAnimMontage(AimMontage);
	}
}

void UBTPS_ShootingMachineComponent::DoFire()
{
	if (!CurrentWeapon || !PlayerCharacter) return;

	if (PlayerCharacter && FireMontage)
	{
		PlayerCharacter->PlayAnimMontage(FireMontage);
	}

	FVector CameraLoc;
	FRotator CameraRot;
	PlayerCharacter->GetController()->GetPlayerViewPoint(CameraLoc, CameraRot);

	float Range = 10000.0f;
	if (CurrentWeapon) Range = CurrentWeapon->AttackRange;

	FVector TraceStart = CameraLoc;
	FVector TraceEnd = CameraLoc + (CameraRot.Vector() * Range);

	FHitResult ScreenHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);
	Params.AddIgnoredActor(CurrentWeapon);

	bool bScreenHit = GetWorld()->LineTraceSingleByChannel(
		ScreenHit, TraceStart, TraceEnd, ECC_Visibility, Params
	);

	FVector TargetLoc = bScreenHit ? ScreenHit.ImpactPoint : TraceEnd;

	FVector MuzzleLoc = CurrentWeapon->GetMuzzleLocation();
	FVector ShootDir = (TargetLoc - MuzzleLoc).GetSafeNormal();
	FVector RealEnd = MuzzleLoc + (ShootDir * Range);

	FHitResult RealHit;
	GetWorld()->LineTraceSingleByChannel(
		RealHit, MuzzleLoc, RealEnd, ECC_Visibility, Params
	);

	if (bDrawFireDebug) 
	{
		DrawDebugLine(GetWorld(), MuzzleLoc, TargetLoc, FColor::Red, false, 1.0f, 0, 1.0f);
	}

	//*데미지 처리
	if (RealHit.bBlockingHit && RealHit.GetActor() && CombatComp)
	{
	CombatComp->ExecuteAttack(
		RealHit.GetActor(),
		10, // 추후 무기 데미지로 변경해주세요! ex: CurrentWeapon->Damage
		RealHit
		);
	}
}

void UBTPS_ShootingMachineComponent::DoInteract()
{
	if (!PlayerCharacter || !GetWorld()) return;

	if (PlayerCharacter && InteractMontage)
	{
		PlayerCharacter->PlayAnimMontage(InteractMontage);
	}

	FVector Start = PlayerCharacter->GetActorLocation();
	FVector End = Start;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractRange);

	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);

	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits, Start, End, FQuat::Identity, ECC_WorldDynamic, Sphere, Params
	);

	if (InteractDebug)
	{
		DrawDebugSphere(GetWorld(), Start, InteractRange, 12, FColor::Green, false, 1.0f);
	}

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			ABTPS_WeaponBase* PickedWeapon = Cast<ABTPS_WeaponBase>(HitActor);

			if (PickedWeapon)
			{
				EquipWeapon(PickedWeapon);
				break; //일단 하나만 줍기
			}
		}
	}
}

void UBTPS_ShootingMachineComponent::DoToggleCamera()
{
	if (!PlayerCharacter) return;

	USpringArmComponent* SpringArm = PlayerCharacter->FindComponentByClass<USpringArmComponent>();
	if (!SpringArm) return;

	bIsFirstPerson = !bIsFirstPerson;

	if (bIsFirstPerson)
	{
		// [FPS 모드]
		SpringArm->TargetArmLength = FPSArmLength;
		SpringArm->bUsePawnControlRotation = true;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}
	else
	{
		// [TPS 모드]
		SpringArm->TargetArmLength = TPSArmLength;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}
}

void UBTPS_ShootingMachineComponent::EquipWeapon(ABTPS_WeaponBase* NewWeapon)
{
	if (!NewWeapon || !PlayerCharacter) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}

	CurrentWeapon = NewWeapon;

	CurrentWeapon->OnEquipped();

	CurrentWeapon->AttachToComponent(
		PlayerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		RightHandSocketName
	);
}