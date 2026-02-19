
#include "03_Components/BTPS_TouchDamageComponent.h"
#include "03_Components/BTPS_StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"


UBTPS_TouchDamageComponent::UBTPS_TouchDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	InitSphereRadius(60.0f);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UBTPS_TouchDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddDynamic(this, &UBTPS_TouchDamageComponent::OnTouchOverlap);
	
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		UBTPS_StatComponent* OwnerStatComp = 
			MyOwner->FindComponentByClass<UBTPS_StatComponent>();
		
		if (OwnerStatComp)
		{
			OwnerStatComp->OnDeath.AddDynamic
			(this, &UBTPS_TouchDamageComponent::OnOwnerDeath);
		}
	}
}

void UBTPS_TouchDamageComponent::OnOwnerDeath()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void UBTPS_TouchDamageComponent::OnTouchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = GetOwner();
	
	if (!MyOwner || !OtherActor || OtherActor == MyOwner) return;
	
	IGenericTeamAgentInterface* MyTeamAgent = Cast<IGenericTeamAgentInterface>(MyOwner);
	IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherActor);

	bool bCanDamage = true;
	
	if (MyTeamAgent && OtherTeamAgent)
	{
		if (MyTeamAgent->GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId())
		{
			bCanDamage = false;
		}
	}

	if (bCanDamage)
	{
		APawn* OwnerPawn = Cast<APawn>(MyOwner);
		AController* InstigatorCtrl = OwnerPawn ? OwnerPawn->GetController() : nullptr;
		
		UGameplayStatics::ApplyDamage(
			OtherActor, 
			DamageAmount, 
			InstigatorCtrl, 
			MyOwner, 
			UDamageType::StaticClass()
		);

		UE_LOG(LogTemp, Warning, TEXT("[TouchDamage] %s dealt %f damage to %s!"), *MyOwner->GetName(), DamageAmount, *OtherActor->GetName());
	}
}
