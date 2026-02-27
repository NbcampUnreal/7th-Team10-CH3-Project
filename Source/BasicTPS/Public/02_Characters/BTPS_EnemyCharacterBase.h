#pragma once

#include "CoreMinimal.h"
#include "BTPS_BaseCharacter.h"
#include "BTPS_EnemyCharacterBase.generated.h"

class UWidgetComponent;

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
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* TreeToRun;
	
	virtual void OnDeath() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY()
	AController* LastAttacker;

	static constexpr float VISIBLE_DISTANCE_MAX = 3000.0f;
	static constexpr float VISIBLE_DISTANCE_MAX_SQ = VISIBLE_DISTANCE_MAX * VISIBLE_DISTANCE_MAX;

	static constexpr float FADE_START_DISTANCE = 2000.0f;
	static constexpr float FADE_START_DISTANCE_SQ = FADE_START_DISTANCE * FADE_START_DISTANCE;

	static constexpr float TICK_INTERVAL_THRESHOLD = 1000.0f;
	static constexpr float TICK_INTERVAL_THRESHOLD_SQ = TICK_INTERVAL_THRESHOLD * TICK_INTERVAL_THRESHOLD;

	bool bIsHealthBarFullyOpaque = false;

	FTimerHandle DistanceCheckTimer;
	void CheckDistanceFromCamera();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const { return TreeToRun; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void SetTarget(AActor* NewTarget);
	virtual AActor* GetTarget() const;
	virtual bool HasTarget() const;
	virtual void ClearTarget();

};
