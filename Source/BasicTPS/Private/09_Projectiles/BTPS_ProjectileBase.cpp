#include "09_Projectiles/BTPS_ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ABTPS_ProjectileBase::ABTPS_ProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionRadius = 15.f;
	CollisionInitialSpeed = 2000.f;
	CollisionMaxSpeed = 2000.f;
	CollisionGravityScale = 1.f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponenet"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(CollisionRadius);
	CollisionComp->OnComponentHit.AddDynamic(this, &ABTPS_ProjectileBase::OnHit);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetEnableGravity(true);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = CollisionInitialSpeed;
	ProjectileMovement->MaxSpeed = CollisionMaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = CollisionGravityScale;

	InitialLifeSpan = 3.f;
}




void ABTPS_ProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		//TODO_CSH 충돌 시 이동변경이 필요하다면 항목 추가(도전 이후 단계)
	}
}

void ABTPS_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}



