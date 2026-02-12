#include "02_Characters/BTPS_Player_AnimInstance.h"
#include "02_Characters/BTPS_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBTPS_Player_AnimInstance::NativeUpdateAnimation(float DeltaSeconsds)
{
	Super::NativeUpdateAnimation(DeltaSeconsds);

	ABTPS_PlayerCharacter* Character = Cast<ABTPS_PlayerCharacter>(TryGetPawnOwner());
	if (!Character) return;

	Velocity = Character->GetVelocity();

	GroundSpeed = Velocity.Length();

	bSouldMove = (GroundSpeed > 3.f 
		&& !Character
		->GetCharacterMovement()
		->GetCurrentAcceleration()
		.IsNearlyZero());

	bIsFalling = Character->GetCharacterMovement()->IsFalling();
}