// OEF 2021

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

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	// Attack / Smash collider
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OurAttack;

	// Where attack hitbox should be placed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector AttackPlacement{70.f, 0.f, 35.f};

	// Using MaxWalkSpeed and CustomMovement from CharacterMovement to set these
	// So no need for UPROPERTY now.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed{ 0.f };
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRunSpeed{ 1000.f };

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

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

	void StartSmash();
	void StopSmash();

	void SwitchInputType();

	//Help variables for the smashing - only needed because we have no attack-animation
	bool isSmashing{ false };
	FVector temp{ 1.f };

	bool normalInputType{ true };		//Used to switch between WASD input behavior
};
