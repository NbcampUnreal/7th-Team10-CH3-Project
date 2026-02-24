// Fill out your copyright notice in the Description page of Project Settings.


#include "04_Items/A_Equipment/Weapon/BTPS_Bullet.h"
#include <components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABTPS_Bullet::ABTPS_Bullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionComp->SetSphereRadius(13.f);
	RootComponent = CollisionComp;

	BulletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMeshComp->SetupAttachment(CollisionComp);
	BulletMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletMeshComp->SetRelativeScale3D(FVector(0.25f));

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(CollisionComp);
	MovementComp->InitialSpeed = 5000;
	MovementComp->MaxSpeed = 5000;
	MovementComp->bShouldBounce = false;
	
	//MovementComp->bShouldBounce = true;
	//MovementComp->Bounciness = 0.3f;

	//InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABTPS_Bullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(
		DeathTimer,
		FTimerDelegate::CreateLambda(
			[this]()-> void
			{
				Destroy();
			}),
		2.0f,
		false
	);
}

// Called every frame
void ABTPS_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABTPS_Bullet::Die()
{
	Destroy();
}
