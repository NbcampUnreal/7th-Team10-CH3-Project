#include "04_Items/A_Equipment/Weapon/BTPS_WeaponBase.h"
#include "Components/SphereComponent.h"

ABTPS_WeaponBase::ABTPS_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//메시(루트)
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	//기본적으로 physics on
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionProfileName(TEXT("PhysicsActor")); //Collision Presets(콜리전 프리셋) 세팅

	//총 줍기 범위
	PickupArea = CreateDefaultSubobject<USphereComponent>(TEXT("PickupArea"));
	PickupArea->SetupAttachment(RootComponent);
	PickupArea->SetSphereRadius(100.0f);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 캐릭터만 감지
}

void ABTPS_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

//총이 땅에 떨어져 있을 때 (physics O)
void ABTPS_WeaponBase::OnDropped()
{
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// 플레이어가 장착했을 때 (physics X)
void ABTPS_WeaponBase::OnEquipped()
{
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

//Muzzle(총구) 소켓이 있으면 총구 위치 반환, 없으면 총(총의 중심) 반환 
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