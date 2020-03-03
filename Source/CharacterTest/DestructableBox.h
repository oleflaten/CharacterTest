// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructableBox.generated.h"

UCLASS()
class CHARACTERTEST_API ADestructableBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructableBox();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ImHit();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OurCollider;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* OurVisibleMesh;

	// What particle system to use when destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UParticleSystem* ExplotionFX{ nullptr };

	// What sound to use when destroyed
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	class USoundBase* CrashSound{nullptr};


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
