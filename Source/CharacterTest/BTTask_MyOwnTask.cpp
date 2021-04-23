// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MyOwnTask.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MyOwnTask::UBTTask_MyOwnTask()
{
	NodeName = TEXT("MyOwnTask");	//Sets the name used in BehaviorTree
}

EBTNodeResult::Type UBTTask_MyOwnTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerSpotted"), true);

	return EBTNodeResult::Succeeded; //return InProgress if Tick should be called
}
