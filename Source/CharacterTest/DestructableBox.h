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

	void ImHit();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OurCollider{nullptr};

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* OurVisibleMesh{nullptr};

	// What particle system to use when destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UParticleSystem* ExplotionFX{ nullptr };

	// What sound to use when destroyed
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	class USoundBase* CrashSound{nullptr};

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float Health{40.f};
	
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
