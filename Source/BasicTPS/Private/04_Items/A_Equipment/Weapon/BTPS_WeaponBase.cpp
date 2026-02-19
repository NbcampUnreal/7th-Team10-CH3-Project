#include "04_Items/A_Equipment/Weapon/BTPS_WeaponBase.h"
#include "Components/SphereComponent.h"

ABTPS_WeaponBase::ABTPS_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	PickupArea = CreateDefaultSubobject<USphereComponent>(TEXT("PickupArea"));
	PickupArea->SetupAttachment(RootComponent);
	PickupArea->SetSphereRadius(100.0f);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABTPS_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABTPS_WeaponBase::ShootAmmo()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}
	return;
}

bool ABTPS_WeaponBase::Reload()
{
	if (CurrentAmmo == MaxAmmo || ReserveAmmo <= 0) return false;

	int32 AmmoNeeded = MaxAmmo - CurrentAmmo;

	if (ReserveAmmo >= AmmoNeeded)
	{
		CurrentAmmo = MaxAmmo;
		ReserveAmmo -= AmmoNeeded;
	}
	else
	{
		CurrentAmmo += ReserveAmmo;
		ReserveAmmo = 0;
	}

	return true;
}

void ABTPS_WeaponBase::OnDropped()
{
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABTPS_WeaponBase::OnEquipped()
{
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector ABTPS_WeaponBase::GetMuzzleLocation() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(MuzzleSocketName))
	{
		return WeaponMesh->GetSocketLocation(MuzzleSocketName);
	}
	return GetActorLocation();
}

/*
TODO.에디터 추가작업
1.무기 상속받아서 BP에서 Mesh설정
2.Physics: Simulate Physics 켜져있는지 확인
3.Collision: Collision Presets를 PhysicsActor로 되어있는지 확인
4.Socket: 총구에 Muzzle소켓 추가.
*/