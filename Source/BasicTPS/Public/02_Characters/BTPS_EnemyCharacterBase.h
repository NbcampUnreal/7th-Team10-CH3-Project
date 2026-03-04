#pragma once

#include "CoreMinimal.h"
#include "BTPS_BaseCharacter.h"
#include "BTPS_EnemyCharacterBase.generated.h"

class UWidgetComponent;
class UDecalComponent;

UCLASS()
class BASICTPS_API ABTPS_EnemyCharacterBase : public ABTPS_BaseCharacter
{
	GENERATED_BODY()

public:
	ABTPS_EnemyCharacterBase();

protected:
	bool bCanAttack = true;
	
	FTimerHandle TouchCooldownTimerHandle;
	FTimerHandle AttackCooldownTimerHandle;
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, Category = "Combat")
	TWeakObjectPtr<AActor> CurrentTarget;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr <class UBehaviorTree> TreeToRun;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UDecalComponent> VisionConeDecal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> VisionMatInstance;
	
	UPROPERTY()
	AController* LastAttacker;
	
	UPROPERTY()
	TObjectPtr<class UBTPS_TouchDamageComponent> TouchDamageComp;
	
	UFUNCTION()
	void OnTouchCompOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	void EnableTouchCollision();
	void ResetAttackCooldown();
	
	virtual void OnDeath() override;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float AttackInterval = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float SightRadius = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float LoseSightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float PeripheralVisionAngle = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float DecalFadeMaxDistance = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	float DecalFadeMinDistance = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	FLinearColor NormalVisionColor = 
		FLinearColor(0.0f, 1.0f, 0.0f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	FLinearColor AlertVisionColor = 
		FLinearColor(1.0f, 0.0f, 0.0f, 0.5f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Info")
	FText EnemyName;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const { return TreeToRun; }

	void StartAttackCooldown();
	bool IsAttackReady() const {return bCanAttack;}
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void SetTarget(AActor* NewTarget);
	virtual AActor* GetTarget() const;
	virtual bool HasTarget() const;
	virtual void ClearTarget();
	virtual void OnConstruction(const FTransform& Transform) override;
	

};
