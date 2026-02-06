#include "Skill/SkillComponent.h"


USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

bool USkillComponent::CanActivate() const
{
	return bCanActivate;
}

void USkillComponent::TryActivate()
{
	if (!CanActivate()) return;

	// TODO SkillCost는 관련 자원이 완료되고 정립, 체크는 Character과 연동필요

	bCanActivate = false;
	CurrentCoolTime = SkillCoolTime;

	SkillActivation();
}

void USkillComponent::SkillActivation() {}