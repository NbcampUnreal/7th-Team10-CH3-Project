#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BTPS_Player_AnimInstance.generated.h"


UCLASS(Transient, Blueprintable)
class BASICTPS_API UBTPS_Player_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|MovementData")
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|MovementData")
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|MovementData")
	bool bSouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|MovementData")
	bool bIsFalling;

public:
	void NativeUpdateAnimation(float DeltaSeconsds) override;
};
