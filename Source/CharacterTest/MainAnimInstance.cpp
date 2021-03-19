// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	if (MyPawn == nullptr)
	{
		MyPawn = TryGetPawnOwner();
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (MyPawn == nullptr)
	{
		MyPawn = TryGetPawnOwner();
	}

	if (MyPawn)
	{
		FVector TempSpeed = MyPawn->GetVelocity();
		FVector LateralSpeed = FVector(TempSpeed.X, TempSpeed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = MyPawn->GetMovementComponent()->IsFalling();
		bIsCrouching = MyPawn->GetMovementComponent()->IsCrouching();
		bIsAttacking = Cast<AMainCharacter>(MyPawn)->bIsAttacking;
	}
}
