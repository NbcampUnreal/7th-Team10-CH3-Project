#pragma once

#include "CoreMinimal.h"
#include "BTPS_BaseCharacter.h"
#include "BTPS_EnemyCharacterBase.generated.h"

UCLASS()
class BASICTPS_API ABTPS_EnemyCharacterBase : public ABTPS_BaseCharacter
{
	GENERATED_BODY()

public:
	ABTPS_EnemyCharacterBase();

protected:
	virtual void BeginPlay() override;

	//AI관련 정리 후 주석 해제하여 사용
	//virtual void PossessedBy(AController* NewController) override;
	//virtual void UnPossessed() override;

	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	TWeakObjectPtr<AActor> CurrentTarget;
	
	virtual void OnDeath() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetTarget(AActor* NewTarget);
	virtual AActor* GetTarget() const;
	virtual bool HasTarget() const;
	virtual void ClearTarget();

};
