// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if(EnemyAIBehavior) //this will work, since I set the pointer to be nullptr, if not set.
	{
		RunBehaviorTree(EnemyAIBehavior); //Runs the selected BT
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("HomeLocation"), GetPawn()->GetActorLocation());
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Enemy: BehaviorTree not found"))
}

void AEnemyAIController::PlayerSpotted(bool IsSpotted)
{
	if(IsSpotted)
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerSpotted"), true);
	}
	else
		GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerSpotted"), false);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	if(GetBlackboardComponent()->GetValueAsBool(TEXT("PlayerSpotted")))	//LineOfSightTo(PlayerPawn))
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
}
