// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MyOwnTask.generated.h"

/** 
* Task are leaf nodes of behavior tree, which perform actual actions
*
* Because some of them can be instanced for specific AI, following virtual functions are not marked as const:
*  - ExecuteTask
*  - AbortTask
*  - TickTask
*  - OnMessage
*
* If your node is not being instanced (default behavior), DO NOT change any properties of object within those functions!
* Template nodes are shared across all behavior tree components using the same tree asset and must store
* their runtime properties in provided NodeMemory block (allocation size determined by GetInstanceMemorySize() )
*
*/
UCLASS()
class CHARACTERTEST_API UBTTask_MyOwnTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_MyOwnTask();

protected:
	/** starts this task, should return Succeeded, Failed or InProgress
	*  (use FinishLatentTask() when returning InProgress)
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
