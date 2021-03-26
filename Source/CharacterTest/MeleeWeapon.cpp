// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DestructableBox.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBase = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	SetRootComponent(RootBase);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootBase);
	WeaponMesh->SetGenerateOverlapEvents(false);	//else this also might trigger overlap events

	AttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackCollider->InitSphereRadius(15.f);
	AttackCollider->SetupAttachment(RootBase);
	AttackCollider->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	//Sometimes setting OnComponentBeginOverlap in constructor does not work inside the editor...	
	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlap);
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Components/UPrimitiveComponent/OnComponentBeginOverlap/index.html
void AMeleeWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{	
	UE_LOG(LogTemp, Warning, TEXT("Enemy Overlaps %s"), *OtherActor->GetName())

	if (OtherActor->IsA(ADestructableBox::StaticClass()))
	{
		//Set up some variables to apply damage:
		FHitResult Hit;
		FPointDamageEvent DamageEvent(Damage, Hit, FVector::ForwardVector, nullptr);

		Cast<ADestructableBox>(OtherActor)->TakeDamage(Damage, DamageEvent, GetOwner()->GetInstigatorController(), this);	
	}
}

