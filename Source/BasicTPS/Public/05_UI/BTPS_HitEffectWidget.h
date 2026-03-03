#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "03_Components/BTPS_StatComponent.h"
#include "BTPS_HitEffectWidget.generated.h"


UCLASS()
class BASICTPS_API UBTPS_HitEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:

	UFUNCTION()
	void HandleHPChanger(float CurrentHP, float MaxHP);

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* HitAnim;

private:
	float LastHP;

	UPROPERTY()
	TObjectPtr<UBTPS_StatComponent> StatComp;
};