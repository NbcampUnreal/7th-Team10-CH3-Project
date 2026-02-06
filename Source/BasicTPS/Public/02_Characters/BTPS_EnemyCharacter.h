#pragma once

#include "CoreMinimal.h"
#include "BTPS_BaseCharacter.h"
#include "BTPS_EnemyCharacter.generated.h"

UCLASS()
class BASICTPS_API ABTPS_EnemyCharacter : public ABTPS_BaseCharacter
{
	GENERATED_BODY()

public:
	ABTPS_EnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
