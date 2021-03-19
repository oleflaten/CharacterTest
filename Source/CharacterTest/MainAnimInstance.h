// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties )
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* MyPawn{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsCrouching{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsAttacking{ false };


};
