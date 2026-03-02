#include "05_UI/BTPS_HUD.h"
#include "05_UI/BTPS_GrenadeSlotWidget.h"
#include "05_UI/BTPS_MainWidget.h"
#include "02_Characters/BTPS_PlayerCharacter.h"
#include "03_Components/BTPS_StatComponent.h"
#include "08_Skill/BTPS_GrenadeSkill.h"

void ABTPS_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("L_MenuLevel"))
	{
		return;  
	}
	
	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UBTPS_MainWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
			
			APlayerController* PC = GetOwningPlayerController();
			if (PC && PC->GetPawn())
			{
				ABTPS_PlayerCharacter* PlayerChar = Cast<ABTPS_PlayerCharacter>(PC->GetPawn());
              
				if (PlayerChar && PlayerChar->GetStatComp())
				{

					MainWidget->BindStatComp(PlayerChar->GetStatComp());
				}

				if (PlayerChar && PlayerChar->GetShootingComp())
				{
					MainWidget->BindShootingComp(PlayerChar->GetShootingComp());
				}
				
				UBTPS_GrenadeSkill* GrenadeComp = PC->GetPawn()
					->FindComponentByClass<UBTPS_GrenadeSkill>();
				if (GrenadeComp && MainWidget->GetGrenadeSlot())
				{
					MainWidget->GetGrenadeSlot()->BindSkillComponent(GrenadeComp);
				}
			}
		}
	}
}