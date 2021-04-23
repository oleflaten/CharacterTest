// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* EnemyAIBehavior{nullptr};

	class APawn* PlayerPawn{nullptr};

protected:
	virtual void BeginPlay() override;

public:
	void PlayerSpotted(bool IsSpotted);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UAIPerceptionComponent* OurPercetionComp{nullptr};
	
};
