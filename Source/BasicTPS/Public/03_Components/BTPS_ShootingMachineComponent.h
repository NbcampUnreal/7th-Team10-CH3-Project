#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "BTPS_ShootingMachineComponent.generated.h"

class UInputAction;
class ABTPS_WeaponBase;
class UBTPS_CombatComponent;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASICTPS_API UBTPS_ShootingMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBTPS_ShootingMachineComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* AimMontage;
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* InteractMontage;

	UPROPERTY(VisibleInstanceOnly, Category = "Aim")
	bool bIsAiming = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float FireRange = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TEnumAsByte<ECollisionChannel> WeaponTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName RightHandSocketName = TEXT("hand_r");

	UPROPERTY(VisibleInstanceOnly, Category = "Weapon")
	ABTPS_WeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Interact")
	FName LeftHandSocketName = TEXT("hand_l");

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bDrawFireDebug = true;
	UPROPERTY(EditAnywhere, Category = "Interact")
	float FireDebugDuration = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractRange = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Interact")
	TEnumAsByte<ECollisionChannel> InteractTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractSphereRadius = 25.0f;
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractDebug = true;
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractDebugDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	bool bIsFirstPerson = false;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float FPSArmLength = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TPSArmLength = 300.0f;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AimStarted(const FInputActionValue& Value);
	void AimCompleted(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void ToggleCamera(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAimStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAimEnd();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoFire();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoInteract();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoToggleCamera();

private:
	UPROPERTY()
	TObjectPtr<UBTPS_CombatComponent> CombatComp;
	
	UPROPERTY()
	ACharacter* PlayerCharacter;

	void EquipWeapon(ABTPS_WeaponBase* NewWeapon);
};
