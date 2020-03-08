// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class CHARACTERTEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void AttackFinished();

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	// Attack collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colliders")
	class UBoxComponent* AttackCollider;

	// Using MaxWalkSpeed and CustomMovement from CharacterMovement to set these
	// So no need for UPROPERTY now.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed{ 0.f };
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRunSpeed{ 1000.f };

	// Bool set to whether we attack or not
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anims")
	bool bIsAttacking{ false };

	// The Animation Montage that contains our attack animations
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage{ nullptr };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Functions triggered by input
	void MoveForward(float Value);
	void MoveRight(float Value);

	void StartCrouch();
	void StopCrouch();

	void StartRun();
	void StopRun();

	void StartAttack();
	void StopAttack();

	void SwitchInputType();

	bool bNormalInputType{ true };		//Used to switch between WASD input behavior
};
