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

void UBTPS_SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	TArray<UBTPS_SkillComponent*> FoundSkills;
	Owner->GetComponents< UBTPS_SkillComponent>(FoundSkills);
	
	for (UBTPS_SkillComponent* Skill : FoundSkills)
	{
		if (Skill != this)
		{
			SkillMap.Add(Skill->GetSkillType(), Skill);
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

	// TODO_CSH SkillCost는 관련 자원이 완료되고 정립, 체크는 Character과 연동필요

	bCanActivate = false;
	CurrentCoolTime = SkillCoolTime;

	SkillActivation();
}

void UBTPS_SkillComponent::ActivateSkill(ESkillType Type)
{
	if (UBTPS_SkillComponent** Found = SkillMap.Find(Type))
	{
		(*Found)->SkillActivation();
	}
}

void UBTPS_SkillComponent::SkillActivation() {}