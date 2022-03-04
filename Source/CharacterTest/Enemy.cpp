// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"	//Unreals AIController class
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
	AIController = Cast<AAIController>(GetController());

	//Had to put these here and not in the constructor, to get them to work
	PlayerSensingSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	PlayerSensingSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		Player->SetEnemy(this);

		//AIController->MoveToActor(Player, 100);

		//or More elaborate way, but gives us more insight:
		MoveToTarget(Player);
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
	int32 OtherBodyIndex)
{
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (AIController && Player)
	{
		Player->SetEnemy(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("Player Ends Overlap"));
		AIController->StopMovement();
	}

}
void AEnemy::MoveToTarget(AMainCharacter* MainCharacter)
{
	//have to include "AIModule" in ProjectNameBuild.cs file for this to work
	if (AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Overlaps"));

		//https://docs.unrealengine.com/en-US/API/Runtime/AIModule/FAIMoveRequest/index.html
		FAIMoveRequest AIMoverequest;
		AIMoverequest.SetGoalActor(MainCharacter);	//What to move towards
		AIMoverequest.SetAcceptanceRadius(25.f);	//How close before stop
	
		//https://docs.unrealengine.com/en-US/API/Runtime/Engine/AI/Navigation/FNavPathSharedPtr/index.html
		FNavPathSharedPtr NavPath;	//Will contain all location nodes for the path
	
		AIController->MoveTo(AIMoverequest, &NavPath);
	
		// **************** this just shows us the path *********************
		//auto guesses the type for us
		auto PathPoints = NavPath->GetPathPoints();
	
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 3, 0.5f);
		}
	}
}


