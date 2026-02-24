// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTPS_BTTask_FireProjectile.generated.h"


UCLASS()
class BASICTPS_API UBTPS_BTTask_FireProjectile : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTPS_BTTask_FireProjectile();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
