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

	UFUNCTION(BlueprintCallable)
	void AttackFinished();

	UFUNCTION(BlueprintCallable)
	void SetAttackCollider(bool AttackColliderOn);

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom{nullptr};

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera{nullptr};

	// Melee Weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AMeleeWeapon> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMeleeWeapon *MyWeapon{nullptr};


	// Using MaxWalkSpeed and CustomMovement from CharacterMovement to set these
	float MaxWalkSpeed{ 0.f };
	float MaxRunSpeed{ 1000.f };

	// Bool set to whether we attack or not
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anims")
	bool bIsAttacking{ false };
							
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

	bool bAttackFinished{false};		//Used to make the whole attack animation run
};
