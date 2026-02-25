#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "BTPS_ShootingMachineComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedDelegate, int32, CurrentAmmo, int32, MaxAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, UTexture2D*, NewIcon);

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* InteractMontage;

	UPROPERTY(VisibleInstanceOnly, Category = "Aim")
	bool bIsAiming = false;
	UPROPERTY(VisibleInstanceOnly, Category = "Aim")
	bool bIsFiring = false;
	float LastFireTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float FireRange = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TEnumAsByte<ECollisionChannel> WeaponTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName RightHandSocketName = TEXT("hand_r");

	UPROPERTY(EditAnywhere, Category = "Interact")
	FName LeftHandSocketName = TEXT("hand_l");

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bDrawFireDebug = true;
	UPROPERTY(EditAnywhere, Category = "Interact")
	float FireDebugDuration = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractRange = 200.0f;

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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChangedDelegate OnAmmoChanged;

	void ToggleAim(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void ToggleCamera(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoToggleAim();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoFire();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoInteract();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoToggleCamera();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoReload();
	
	UPROPERTY(VisibleInstanceOnly, Category = "Weapon")
	ABTPS_WeaponBase* CurrentWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = "BulletFactory")
	TSubclassOf<class ABTPS_Bullet> BulletFactory;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

	bool IsAiming() const { return bIsAiming; }
	bool IsFiring() const { return bIsFiring; }

	FTimerHandle FireDelayTimer;
	FTimerHandle ActionEndTimer;

	void ExecuteFire();
	void EndFireAction();

private:
	UPROPERTY()
	TObjectPtr<UBTPS_CombatComponent> CombatComp;
	
	UPROPERTY()
	ACharacter* PlayerCharacter;

	void EquipWeapon(ABTPS_WeaponBase* NewWeapon);
};
