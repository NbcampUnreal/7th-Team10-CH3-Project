#include "02_Characters/BTPS_EnemyTemp1.h"


ABTPS_EnemyTemp1::ABTPS_EnemyTemp1()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(Scene);
}

void ABTPS_EnemyTemp1::BeginPlay()
{
	Super::BeginPlay();
}
