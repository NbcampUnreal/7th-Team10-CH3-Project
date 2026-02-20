#include "02_Characters/BTPS_EnemyTemp1.h"


ABTPS_EnemyTemp1::ABTPS_EnemyTemp1()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
}

void ABTPS_EnemyTemp1::BeginPlay()
{
	Super::BeginPlay();
}
