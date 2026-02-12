#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BTPS_BaseCharacter.generated.h"

class UBTPS_StatComponent;
class UBTPS_CombatComponent;

UCLASS()
class BASICTPS_API ABTPS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABTPS_BaseCharacter();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBTPS_StatComponent> StatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBTPS_CombatComponent> CombatComp;
	
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
		) override;
	
	UBTPS_StatComponent* GetStatComp() const { return StatComp; }
	UBTPS_CombatComponent* GetCombatComp() const {return CombatComp;}
};
