#include "09_Projectiles/BTPS_GrenadeProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABTPS_GrenadeProjectile::ABTPS_GrenadeProjectile()
{
	ExplosionRadius = 600.f;
	ExplosionDamage = 300.f;


	ExplosionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionComponent"));
	ExplosionComp->SetupAttachment(RootComponent);
	ExplosionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionComp->InitSphereRadius(ExplosionRadius);

	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->Friction = 0.8f;

	CollisionComp->SetLinearDamping(0.8f);
	CollisionComp->SetAngularDamping(2.f);

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

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionComp->GetUnscaledSphereRadius(), 32, FColor::Red, false, 2.f);

	TArray<AActor*> OverlappingActors;
	ExplosionComp->UpdateOverlaps();
	ExplosionComp->GetOverlappingActors(OverlappingActors);

	UE_LOG(LogTemp, Warning, TEXT("Found %d actors in explosion range"), OverlappingActors.Num());

	AActor* TempOwner = GetOwner();
	if (!TempOwner)
		return;

	APawn* OwningPawn = Cast<APawn>(TempOwner);
	if (!OwningPawn)
		return;

	AController* OwnerController = OwningPawn->GetController();
	if (!OwnerController)
		return;

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != TempOwner)
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				OwnerController,
				this,
				UDamageType::StaticClass()
			);
			UE_LOG(LogTemp, Warning, TEXT("[Grenade] Hit %s! Damage: %f"), *Actor->GetName(), ExplosionDamage);
		}
	}

	Destroy();
}

