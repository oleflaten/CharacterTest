// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OurSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FOurSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category= "SaveGame")
	float Health;

	UPROPERTY(VisibleAnywhere, Category= "SaveGame")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category= "SaveGame")
	int Ammo;
};


UCLASS()
class CHARACTERTEST_API UOurSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Basic)
	FVector PlayerPosition{0.f, 0.f, 0.f};

	UPROPERTY(EditAnywhere, Category = Basic)
	FOurSaveData SaveData;
	
};
