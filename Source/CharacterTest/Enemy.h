// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class CHARACTERTEST_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//A sphere that if the Player enters, triggers the NPC to follow Player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class USphereComponent* PlayerSensingSphere{nullptr};

	//The Controller for the NCP - similar to the PlayerController
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class AAIController* AIController{ nullptr };

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	//Function that actually moves the NPC
	//For now the target to move towards is the MainCharacter - Chase mode
	void MoveToTarget(class AMainCharacter* MainCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float Health{100.f};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
