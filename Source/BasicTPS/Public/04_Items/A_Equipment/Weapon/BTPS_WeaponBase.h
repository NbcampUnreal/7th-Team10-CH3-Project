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
	//무기 외형 (스켈레탈 메시), 총기에서 탄피가 나가거나 하는 애니메이션 등 구현하려면 static mesh말고 skeletal mesh.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	//줍기 감지용 구체 (이 근처에서 상호작용키 눌러야 함)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* PickupArea;
	/*
	추가 설명, 이번에 ai써가면서 알게 된건데
	USphereComponent* PickupArea;를 하려면
	class USphereComponent를 위에 include아래에 전방선언 해줘야 하잖아요?
	class USphereComponent* PickupArea; << 이런식으로 쓰면 한번에 전방선언을 하면서 선언하는 방식이라고 합니다.
	제미나이 말로는 실무에서 쓴다고는 합니다.
	*/

	//range는 MeleeWeapon으로 뺄 수 있습니다.
	UPROPERTY(EditAnywhere, Category = "Stats")
	float AttackRange = 10000.0f;

	// 총구 소켓 이름 (칼인 경우엔 공격 시작점 등으로 활용 가능)
	UPROPERTY(EditAnywhere, Category = "Effects")
	FName MuzzleSocketName = TEXT("Muzzle");


	virtual void OnDropped();

	virtual void OnEquipped();

	FVector GetMuzzleLocation() const;
};
