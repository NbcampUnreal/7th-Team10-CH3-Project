#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "00_Common/BTPS_CoreTypes.h"
#include "BTPS_SpawnManager.generated.h"

class USceneComponent;
class UBoxComponent;

UCLASS()
class BASICTPS_API ABTPS_SpawnManager : public AActor
{
	GENERATED_BODY()

public:
	ABTPS_SpawnManager();
	
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnRandomEnemy();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	TObjectPtr<UBoxComponent> SpawningBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	UDataTable* EnemyDataTable;
	
	
	FVector GetRandomPointInVolume() const;
	FEnemySpawnRow* GetRandomEnemy() const;
	void SpawnEnemy(TSubclassOf<APawn> EnemyClass);
	

};