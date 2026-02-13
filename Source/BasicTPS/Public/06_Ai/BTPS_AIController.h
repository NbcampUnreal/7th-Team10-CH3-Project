// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BTPS_AIController.generated.h"


class UAISenseConfig_Sight;
class UBehaviorTree;
class UBlackboardComponent;

UCLASS()
class BASICTPS_API ABTPS_AIController : public AAIController
{
	GENERATED_BODY()

public:
	ABTPS_AIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName TargetKeyName = TEXT("TargetActor");
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
