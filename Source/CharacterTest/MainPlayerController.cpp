// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Set up and add Enemy Health Bar
	// if(EnemyWidget)
	// {
	// 	EnemyHealtBar = CreateWidget<UUserWidget>(this, EnemyWidget);
	// 	if(EnemyHealtBar)
	// 	{
	// 		EnemyHealtBar->AddToViewport();
	// 		EnemyHealtBar->SetVisibility(ESlateVisibility::Hidden);
	// 	}
	// }
	// else
	// 	UE_LOG(LogTemp, Error, TEXT("EnemyHealthBar not set"))
}
