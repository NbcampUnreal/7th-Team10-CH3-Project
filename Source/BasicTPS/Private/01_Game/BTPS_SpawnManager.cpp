#include "01_Game/BTPS_SpawnManager.h"
#include "Components/BoxComponent.h"


ABTPS_SpawnManager::ABTPS_SpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	
	SpawningBox = CreateDefaultSubobject<UBoxComponent>((TEXT("SpawningBox")));
	SpawningBox->SetupAttachment(Scene);
}

void ABTPS_SpawnManager::SpawnRandomEnemy()
{
	if (FEnemySpawnRow* SelectedRow = GetRandomEnemy())
	{
		if (UClass* ActualClass = SelectedRow->EnemyClass.Get())
		{
			SpawnEnemy(ActualClass);
		}
	}
}

FVector ABTPS_SpawnManager::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(0.0f,0.0f)
		);
}

FEnemySpawnRow* ABTPS_SpawnManager::GetRandomEnemy() const
{
	TArray<FEnemySpawnRow*> AllRows;
	static const FString ContextString(TEXT("EnemySpawnContext"));
	EnemyDataTable->GetAllRows(ContextString, AllRows);
	
	if (AllRows.IsEmpty()) {return nullptr;}
	
	float TotalChance = 0.0f;
	for (const FEnemySpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	
	const float RandValue = FMath::RandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	
	for (FEnemySpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	
	return nullptr;
}

void ABTPS_SpawnManager::SpawnEnemy(TSubclassOf<APawn> EnemyClass)
{
	if (!EnemyClass) {return;}
	
	GetWorld()->SpawnActor<APawn>(
		EnemyClass,
		GetRandomPointInVolume(),
		FRotator(0.0f, 0.0f, 0.0f)
		);
}

