#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "08_Skill/BTPS_GrenadeSkill.h"
#include "BTPS_GrenadeSlotWidget.generated.h"

UCLASS()
class BASICTPS_API UBTPS_GrenadeSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindSkillComponent(UBTPS_GrenadeSkill* InComponent);

protected:
	virtual void NativeTick(const FGeometry& Geometry, float InDeltaTime) override;

	UFUNCTION()
	void OnCoolTimeFinished();

	UPROPERTY(meta = (BindWidget))
	class UImage* GrenadeImage;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CoolTimeBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoolTimeText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* FinishAnim;

private:
	UPROPERTY()
	TObjectPtr<UBTPS_GrenadeSkill> SkillComponent;
};
