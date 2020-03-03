// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructableBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADestructableBox::ADestructableBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the collider for this object
	OurCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderBox"));
	OurCollider->InitBoxExtent(FVector(50.f));
	// Set OurCollider to be the RootComponent
	RootComponent = OurCollider;

	// Set up our visible mesh
	OurVisibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleMesh"));
	OurVisibleMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADestructableBox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADestructableBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructableBox::ImHit()
{
	//PartikkelFX: - you must the set up a ExplotionFX actor first!
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplotionFX, GetTransform(), true);

	//SoundFX:  - you must the set up a ExplotionSound actor first!
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CrashSound, GetActorLocation());

	Destroy();
}

