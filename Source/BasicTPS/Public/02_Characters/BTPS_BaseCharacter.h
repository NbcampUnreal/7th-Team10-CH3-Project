#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "BTPS_BaseCharacter.generated.h"

class UBTPS_StatComponent;
class UBTPS_CombatComponent;

UCLASS()
class BASICTPS_API ABTPS_BaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABTPS_BaseCharacter();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBTPS_StatComponent> StatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBTPS_CombatComponent> CombatComp;
	
	FGenericTeamId TeamID = FGenericTeamId::NoTeam;
	
	UFUNCTION()
	virtual void OnDeath();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsDead = false;
	

	
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
		) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageText")
	float TextOffsetZ = 150.F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageText")
	float TextOffsetY = 50.f;
	FVector DamageTextZOffset = FVector(0.f, 0.f, TextOffsetZ);
	FVector DamageTextYOffset = FVector(0.f, TextOffsetY, 0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageText")
	TObjectPtr<class UNiagaraSystem> DamageTextSystem;
	UFUNCTION()
	void ShowDamageText(float DamageAmount);
	
	UBTPS_StatComponent* GetStatComp() const { return StatComp; }
	UBTPS_CombatComponent* GetCombatComp() const {return CombatComp;}
};
