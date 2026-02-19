#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTPS_WeaponBase.generated.h"

UCLASS()
class BASICTPS_API ABTPS_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABTPS_WeaponBase();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* PickupArea;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float AttackRange = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	FName MuzzleSocketName = TEXT("Muzzle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 MaxAmmo = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 CurrentAmmo = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 ReserveAmmo = 90;

	int32 GetMaxAmmo() const { return MaxAmmo; }
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	virtual void ShootAmmo();

	virtual bool Reload();

	virtual void OnDropped();

	virtual void OnEquipped();

	FVector GetMuzzleLocation() const;
};
