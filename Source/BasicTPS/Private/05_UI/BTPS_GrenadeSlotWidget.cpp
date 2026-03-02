#include "05_UI/BTPS_GrenadeSlotWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Internationalization/Text.h"


void UBTPS_GrenadeSlotWidget::BindSkillComponent(UBTPS_GrenadeSkill* InComponent)
{
	if (InComponent)
	{
		SkillComponent = InComponent;
		SkillComponent->OnCoolTimeFinished.AddDynamic(this, &UBTPS_GrenadeSlotWidget::OnCoolTimeFinished);
	}
}

void UBTPS_GrenadeSlotWidget::NativeTick(const FGeometry& Geometry, float InDeltaTime)
{
	Super::NativeTick(Geometry, InDeltaTime);

	if (SkillComponent && CoolTimeBar)
	{
		float MaxCool = SkillComponent->GetMaxCoolTime();
		if (MaxCool > 0.f)
		{
			float Ratio = SkillComponent->GetCurrentCoolTime() / MaxCool;
			CoolTimeBar->SetPercent(Ratio);
		}
	}

	if (SkillComponent && CoolTimeText)
	{
		float Current = SkillComponent->GetCurrentCoolTime();

		if (Current > 0.f)
		{
			CoolTimeText->SetVisibility(ESlateVisibility::HitTestInvisible);

			FNumberFormattingOptions Opts;

			if (Current >= 2.f)
			{
				Opts.MaximumFractionalDigits = 0;
				Opts.MinimumFractionalDigits = 0;

				CoolTimeText->SetColorAndOpacity(FLinearColor::White);
			}
			else
			{
				Opts.MaximumFractionalDigits = 1;
				Opts.MinimumFractionalDigits = 1;

				CoolTimeText->SetColorAndOpacity(FLinearColor::Yellow);
			}

			FText TargetText = FText::AsNumber(Current, &Opts);
			CoolTimeText->SetText(TargetText);
		}
		else
		{
			CoolTimeText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBTPS_GrenadeSlotWidget::OnCoolTimeFinished()
{
	if (FinishAnim)
	{
		PlayAnimation(FinishAnim);
	}
}
