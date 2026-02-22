#include "09_Projectiles/BTPS_GrenadeProjectile.h"
#include "02_Characters/BTPS_PlayerController.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABTPS_GrenadeProjectile::ABTPS_GrenadeProjectile()
{
	ExplosionRadius = 400.f;
	ExplosionDamage = 30.f;

	ExplosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionComponent"));
	ExplosionComp->SetupAttachment(RootComponent);
	ExplosionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionComp->InitSphereRadius(ExplosionRadius);

	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.5f;
	InitialLifeSpan = 4.f;
}

void ABTPS_GrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		FuseTimerHandle,
		this,
		&ABTPS_GrenadeProjectile::Explode,
		FuseTime,
		false
	);
}

void ABTPS_GrenadeProjectile::Explode()
{
	if (bExploded)
		return;

	bExploded = true;

	GetWorld()->GetTimerManager().ClearTimer(FuseTimerHandle);

	TArray<AActor*> OverlappingActors;

	ExplosionComp->GetOverlappingActors(OverlappingActors);

	AController* OwnerController = Cast<ABTPS_PlayerController>(GetOwner());
	if (!OwnerController)
		return;

	for (AActor* Actor : OverlappingActors)
	{
		//TODO_CSH 데미지를 받을 예외처리 구문을 위한 태그 추가 필요, 추가시 여기에서 예외처리
		UGameplayStatics::ApplyDamage(
			Actor,
			ExplosionDamage,
			OwnerController,
			this,
			UDamageType::StaticClass()
		);
	}

	Destroy();
}

