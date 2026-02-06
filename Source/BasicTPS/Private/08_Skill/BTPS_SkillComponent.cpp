#include "08_Skill/BTPS_SkillComponent.h"


UBTPS_SkillComponent::UBTPS_SkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBTPS_SkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bCanActivate)
	{
		CurrentCoolTime -= DeltaTime;

		if (CurrentCoolTime <= 0.f)
		{
			CurrentCoolTime = 0.f;
			bCanActivate = true;
		}
	}
}

bool UBTPS_SkillComponent::CanActivate() const
{
	return bCanActivate;
}

void UBTPS_SkillComponent::TryActivate()
{
	if (!CanActivate()) return;

	// TODO SkillCost는 관련 자원이 완료되고 정립, 체크는 Character과 연동필요

	bCanActivate = false;
	CurrentCoolTime = SkillCoolTime;

	SkillActivation();
}

void UBTPS_SkillComponent::SkillActivation() {}