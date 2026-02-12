#pragma once

#include "CoreMinimal.h"
#include "02_Characters/BTPS_EnemyCharacterBase.h"
#include "BTPS_EnemyTemp1.generated.h"

UCLASS()
class BASICTPS_API ABTPS_EnemyTemp1 : public ABTPS_EnemyCharacterBase
{
	GENERATED_BODY()

public:
	ABTPS_EnemyTemp1();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
protected:
	virtual void BeginPlay() override;
};