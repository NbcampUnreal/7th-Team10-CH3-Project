#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASICTPS_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

	bool CanActivate() const;

	void TryActivate();

protected:
	int32 SkillCost = 0;
	float SkillCoolTime = 0;
	float CurrentCoolTime = 0;
	bool bCanActivate = true;


protected:
	virtual void SkillActivation();


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
