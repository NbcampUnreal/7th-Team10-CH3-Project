// Fill out your copyright notice in the Description page of Project Settings.


#include "04_Items/A_Equipment/Weapon/BTPS_Bullet.h"
#include <components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
ABTPS_Bullet::ABTPS_Bullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionComp->SetSphereRadius(13.f);
	CollisionComp->SetNotifyRigidBodyCollision(true);
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

	CollisionComp->OnComponentHit.AddDynamic(this, &ABTPS_Bullet::OnHit);
	
	if (GetInstigator())
	{
		CollisionComp->IgnoreActorWhenMoving(GetInstigator(), true);
	}
	if (GetOwner())
	{
		CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	}
	
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

void ABTPS_Bullet::OnHit(
	UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, 
	const FHitResult& Hit)
{
	FString ActorName = OtherActor ? OtherActor->GetName() : TEXT("None");
	FString CompName = OtherComp ? OtherComp->GetName() : TEXT("None");
	UE_LOG(LogTemp, Error, TEXT("Bullet Hit! Actor: %s / Component: %s"), *ActorName, *CompName);
	
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,                 
			BulletDamage,               
			GetInstigatorController(),  
			this,                       
			UDamageType::StaticClass()  
		);
	}
    
	// 적중 후 총알 파괴
	Destroy();
}

void ABTPS_Bullet::Die()
{
	Destroy();
}
