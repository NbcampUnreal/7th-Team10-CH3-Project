// Fill out your copyright notice in the Description page of Project Settings.


#include "06_Ai/BTPS_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTPS_ClearBlackboardValue::UBTPS_ClearBlackboardValue()
{
	NodeName = TEXT("Clear BB Value");
}

EBTNodeResult::Type UBTPS_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->ClearValue(GetSelectedBlackboardKey());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
