#include "05_UI/BTPS_HitEffectWidget.h"
#include "Animation/WidgetAnimation.h"

void UBTPS_HitEffectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (OwningPawn)
	{
		StatComp = OwningPawn->FindComponentByClass<UBTPS_StatComponent>();
		if (StatComp)
		{
			LastHP = StatComp->GetCurrentHP();

			StatComp->OnHPChanged.AddDynamic(this, &UBTPS_HitEffectWidget::HandleHPChanger);
		}
	}
}

void UBTPS_HitEffectWidget::HandleHPChanger(float CurrentHP, float MaxHP)
{
	if (CurrentHP < LastHP && HitAnim)
	{
		PlayAnimation(HitAnim);
	}

	LastHP = CurrentHP;
}
