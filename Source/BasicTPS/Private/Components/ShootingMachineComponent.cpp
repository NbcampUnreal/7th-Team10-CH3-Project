#include "Components/ShootingMachineComponent.h"
#include "EnhancedInputComponent.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
// #include "CollisionAndTrace5_6Character.h" 팀플 camera component구현한 헤더 포함해야함, camera component는 public으로 해야함.


UShootingMachineComponent::UShootingMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UShootingMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ACharacter>(GetOwner());
}


void UShootingMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

/* action binding할때 여기 헤더 참조해서 같이 바인딩.
void UShootingMachineComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		if (AimAction)
		{
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &UShootingMachineComponent::AimStarted);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &UShootingMachineComponent::AimCompleted);
		}

		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UShootingMachineComponent::Fire);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UShootingMachineComponent::Interact);
		}
	}
}
*/


void UShootingMachineComponent::AimStarted(const FInputActionValue& Value)
{
	DoAimStart();
}

void UShootingMachineComponent::AimCompleted(const FInputActionValue& Value)
{
	DoAimEnd();
}

void UShootingMachineComponent::Fire(const FInputActionValue& Value)
{
	DoFire();
}

void UShootingMachineComponent::Interact(const FInputActionValue& Value)
{
	DoInteract();
}


//move에 추가 	if (bIsAiming) return;

void UShootingMachineComponent::DoAimStart()
{
	bIsAiming = true;

	if (PlayerCharacter && AimMontage)
	{
		PlayerCharacter->PlayAnimMontage(AimMontage);
	}
}

void UShootingMachineComponent::DoAimEnd()
{
	bIsAiming = false;

	if (PlayerCharacter && AimMontage)
	{
		PlayerCharacter->StopAnimMontage(AimMontage);
	}
}

void UShootingMachineComponent::DoFire() {}
void UShootingMachineComponent::DoInteract() {}

/* camera component확인 후 연결해야함.
void UShootingMachineComponent::DoFire()
{
	if (PlayerCharacter && FireMontage)
	{
		PlayerCharacter->PlayAnimMontage(FireMontage);
	}

	ACollisionAndTrace5_6Character* MyChar = Cast<ACollisionAndTrace5_6Character>(PlayerCharacter);
	if (!MyChar) return;

	USkeletalMeshComponent* MeshComp = MyChar->GetMesh();

	if (!MeshComp || !GetWorld())
	{
		return;
	}

	FVector Start = FVector::ZeroVector;

	if (MeshComp->DoesSocketExist(RightHandSocketName))
	{
		Start = MeshComp->GetSocketLocation(RightHandSocketName);
	}
	else
	{
		const int32 BoneIndext = MeshComp->GetBoneIndex(RightHandSocketName);
		if (BoneIndext != INDEX_NONE)
		{
			Start = MeshComp->GetBoneLocation(RightHandSocketName);
		}
		else
		{
			Start = MyChar->GetActorLocation();
		}
	}

	FVector Dir = MyChar->GetActorForwardVector();
	if (MyChar->FollowCamera)
	{
		Dir = MyChar->FollowCamera->GetForwardVector();
	}

	else if (MyChar->GetController())
	{
		Dir = MyChar->GetController()->GetControlRotation().Vector();
	}

	const FVector End = Start + Dir * FireRange;

	const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(WeaponTraceChannel);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	const EDrawDebugTrace::Type DebugType = bDrawFireDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	FHitResult Hit;

	const bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		TraceType,
		true,
		ActorsToIgnore,
		DebugType,
		Hit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		FireDebugDuration
	);

	if (bHit && Hit.GetActor())
	{
		const FString HitName = Hit.GetActor()->GetName();

		//Hit.GetActor() -> Destroy();
	}
}

void UShootingMachineComponent::DoInteract()
{
	if (PlayerCharacter && InteractMontage)
	{
		PlayerCharacter->PlayAnimMontage(InteractMontage);
	}

	USkeletalMeshComponent* MeshComp = GetMesh();

	if (!MeshComp || !GetWorld())
	{
		return;
	}

	FVector Start = FVector::ZeroVector;

	if (MeshComp->DoesSocketExist(LeftHandSocketName))
	{
		Start = MeshComp->GetSocketLocation(LeftHandSocketName);
	}
	else
	{
		const int32 BoneIndext = MeshComp->GetBoneIndex(LeftHandSocketName);
		if (BoneIndext != INDEX_NONE)
		{
			Start = MeshComp->GetBoneLocation(LeftHandSocketName);
		}
		else
		{
			Start = GetActorLocation();
		}
	}

	FVector Dir = GetActorForwardVector();
	if (FollowCamera)
	{
		Dir = FollowCamera->GetForwardVector();
	}
	else if (Controller)
	{
		Dir = Controller->GetControlRotation().Vector();
	}

	const FVector Center = Start + Dir * InteractRange;

	//FCollisionQueryParams Params(SCENE_QUERY_STAT());

	const FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractSphereRadius);

	FHitResult Hit;

	const bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Center,
		Center,
		FQuat::Identity,
		InteractTraceChannel,
		Sphere
	);

	if (DrawDebugSphere)
	{
		DrawDebugSphere(
			GetWorld(),
			Center,
			InteractSphereRadius,
			16,
			bHit ? FColor::Green : FColor::Red,
			false,
			InteractDebugDuration
		);

		if (bHit)
		{
			DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 12.0f, FColor::Yellow, false, InteractDebugDuration);
		}
	}

	if (bHit && Hit.GetActor())
	{
		AActor* HitActor = Hit.GetActor();
		if (IsValid(HitActor) && HitActor != this)
		{
			const FString HitName = HitActor->GetName();
		}
	}
}
*/