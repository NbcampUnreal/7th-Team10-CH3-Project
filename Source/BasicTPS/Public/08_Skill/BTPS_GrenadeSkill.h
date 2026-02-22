#pragma once

#include "CoreMinimal.h"
#include "08_Skill/BTPS_SkillComponent.h"
#include "09_Projectiles/BTPS_GrenadeProjectile.h"
#include "00_Common/BTPS_CoreTypes.h"
#include "BTPS_GrenadeSkill.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta=(BlueprintSpawnableComponent))
class BASICTPS_API UBTPS_GrenadeSkill : public UBTPS_SkillComponent
{
	GENERATED_BODY()
	
public:
	UBTPS_GrenadeSkill();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade")
	TSubclassOf<ABTPS_GrenadeProjectile> GrenadeClass;

protected:

	void SkillActivation() override;
	ESkillType GetSkillType() const override { return ESkillType::Grenade; }

	void SpawnGrenadeProjectile();
};
