// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class CHARACTERTEST_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//Set the Enemy Health Bar Widget
	//Have to include "UMG" module in Build.cs to use UUserWidget
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	// TSubclassOf<class UUserWidget> EnemyWidget;
	//
	//  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	//  UUserWidget* EnemyHealtBar{nullptr};	

protected:
	virtual void BeginPlay() override;	

};
