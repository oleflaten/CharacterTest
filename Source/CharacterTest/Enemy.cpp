// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "EnemyAIController.h"
#include "MainCharacter.h"	//Since we move towards the Player
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerSensingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerSensingSphere"));
	PlayerSensingSphere->SetupAttachment(GetRootComponent());
	PlayerSensingSphere->InitSphereRadius(650.f);

	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Characters in level will automatically be given an AIController if not given a PlayerController
	AIController = Cast<AEnemyAIController>(GetController());
	if(!AIController)
		UE_LOG(LogTemp, Error, TEXT("Enemy has no AIController!!"));

	//Had to put these here and not in the constructor, to get them to work
	PlayerSensingSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	PlayerSensingSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		AIController->PlayerSpotted(true);
		
		//Easy way shown in GameDev course video 193:
 		// AIController->MoveToActor(Player, 100);
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (AIController && Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Ends Overlap"));
		AIController->PlayerSpotted(false);
	}
}


