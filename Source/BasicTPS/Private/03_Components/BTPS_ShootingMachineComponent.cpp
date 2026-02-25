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
#include "04_Items/A_Equipment/Weapon/BTPS_Bullet.h"
#include "04_Items/A_Equipment/Weapon/BTPS_WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"


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


void UBTPS_ShootingMachineComponent::ToggleAim(const FInputActionValue& Value)
{
	DoToggleAim();
}

void UBTPS_ShootingMachineComponent::Fire(const FInputActionValue& Value)
{
	if (!CurrentWeapon || !PlayerCharacter) return;
	if (CurrentWeapon->GetCurrentAmmo() <= 0) return;

	if (PlayerCharacter && FireMontage)
	{
		PlayerCharacter->PlayAnimMontage(FireMontage);
	}

	bIsFiring = true;

	if (PlayerCharacter->GetCharacterMovement())
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}

	GetWorld()->GetTimerManager().SetTimer(ActionEndTimer, this, &UBTPS_ShootingMachineComponent::EndFireAction, 0.5f, false);

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (bIsAiming || (CurrentTime - LastFireTime < 0.7f))
	{
		DoFire();
	}
	else
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(FireDelayTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(FireDelayTimer, this, &UBTPS_ShootingMachineComponent::DoFire, 0.25f, false);
		}
	}
}

void UBTPS_ShootingMachineComponent::Interact(const FInputActionValue& Value)
{
	DoInteract();
}

void UBTPS_ShootingMachineComponent::ToggleCamera(const FInputActionValue& Value)
{
	DoToggleCamera();
}

void UBTPS_ShootingMachineComponent::Reload(const FInputActionValue& Value)
{
	DoReload();
}


//move에 추가 	if (bIsAiming) return;

void UBTPS_ShootingMachineComponent::DoToggleAim()
{
	bIsAiming = !bIsAiming;
}

void UBTPS_ShootingMachineComponent::DoFire()
{
	if (!CurrentWeapon || !PlayerCharacter) return;

	LastFireTime = GetWorld()->GetTimeSeconds();

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

	//총알 발사
	FRotator BulletRotation = ShootDir.Rotation();
	FTransform FirePosition = CurrentWeapon->GetMuzzleTransform();
	FirePosition.SetScale3D(FVector(1.f, 1.f, 1.f));
	FirePosition.SetRotation(BulletRotation.Quaternion()); 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;
	SpawnParams.Instigator = Cast<APawn>(PlayerCharacter);
	
	ABTPS_Bullet* SpawnedBullet = GetWorld()->SpawnActor<ABTPS_Bullet>(BulletFactory, FirePosition, SpawnParams);
	
	
	//*데미지 처리
	if (RealHit.bBlockingHit && RealHit.GetActor() && CombatComp)
	{
	CombatComp->ExecuteAttack(
		RealHit.GetActor(),
		10, // 추후 무기 데미지로 변경해주세요! ex: CurrentWeapon->Damage
		RealHit
		);
	}

	CurrentWeapon->ShootAmmo();
	OnAmmoChanged.Broadcast(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetMaxAmmo());

	if(CurrentWeapon->GetCurrentAmmo() == 0)
	{
		if (CurrentWeapon->Reload())
		{
			OnAmmoChanged.Broadcast(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetMaxAmmo());
		}
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

void UBTPS_ShootingMachineComponent::DoReload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
		OnAmmoChanged.Broadcast(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetMaxAmmo());
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
	if (CurrentWeapon)
	{
		OnAmmoChanged.Broadcast(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetMaxAmmo());
	}
}

void UBTPS_ShootingMachineComponent::EndFireAction()
{
	bIsFiring = false;

	if (PlayerCharacter && PlayerCharacter->GetCharacterMovement())
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
}

void UBTPS_ShootingMachineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireDelayTimer);
		GetWorld()->GetTimerManager().ClearTimer(ActionEndTimer);
	}

	OnAmmoChanged.Clear();
	OnWeaponChanged.Clear();

	Super::EndPlay(EndPlayReason);
}