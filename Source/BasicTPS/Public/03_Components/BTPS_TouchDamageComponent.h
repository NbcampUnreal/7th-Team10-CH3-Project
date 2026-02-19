
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BTPS_TouchDamageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASICTPS_API UBTPS_TouchDamageComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UBTPS_TouchDamageComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageAmount = 15.0f;

	UFUNCTION()
	void OnTouchOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	UFUNCTION()
	void OnOwnerDeath();
};
