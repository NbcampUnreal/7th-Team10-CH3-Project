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
	virtual void BeginPlay() override;

	//AI관련 정리 후 주석 해제하여 사용
	//virtual void PossessedBy(AController* NewController) override;
	//virtual void UnPossessed() override;

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
	
	virtual void OnDeath() override;

public:
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
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const { return TreeToRun; }
	
	virtual void SetTarget(AActor* NewTarget);
	virtual AActor* GetTarget() const;
	virtual bool HasTarget() const;
	virtual void ClearTarget();
	virtual void OnConstruction(const FTransform& Transform) override;

};
