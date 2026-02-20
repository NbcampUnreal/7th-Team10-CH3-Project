#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "00_Common/BTPS_CoreTypes.h"
#include "BTPS_SkillComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASICTPS_API UBTPS_SkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBTPS_SkillComponent();

	bool CanActivate() const;

	void TryActivate();

	void ActivateSkill(ESkillType Type);

public:
	UPROPERTY()
	TMap<ESkillType, UBTPS_SkillComponent*> SkillMap;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillCost")
	int32 SkillCost = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillCost")
	float SkillCoolTime = 0;
	float CurrentCoolTime = 0;
	bool bCanActivate = true;


protected:
	virtual void SkillActivation();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual ESkillType GetSkillType() const { return ESkillType::None; }
};
