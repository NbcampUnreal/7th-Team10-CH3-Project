#pragma once

#include "CoreMinimal.h"
#include "BTPS_CoreTypes.generated.h" 


UENUM(BlueprintType)
enum class EXxType : uint8
{
	Ex,
	Xx
};

UENUM(BlueprintType)
enum class EnemyList : uint8
{
	Temp1,
	Temp2,
	Temp3
};

USTRUCT(Atomic, BlueprintType)
struct FXxData
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FEnemySpawnRow : public FTableRowBase
{
	GENERATED_BODY();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EnemyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> EnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance = 1.f;
};

USTRUCT(BlueprintType)
struct FKillLogData
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadOnly)
	FString KillerName;
	UPROPERTY(BlueprintReadOnly)
	FString VictimName;
};