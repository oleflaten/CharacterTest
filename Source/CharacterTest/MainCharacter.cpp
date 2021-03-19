// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DestructableBox.h"
#include "Animation/AnimInstance.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 40.f;
	CameraBoom->SetRelativeRotation(FRotator(0.f, -35.f, 50.f));	//Does not work correctly - set in editor
	CameraBoom->bUsePawnControlRotation = false;	// Ignore controller rotations
	CameraBoom->bInheritYaw = false;				// Ignore Yaw rotations
	CameraBoom->bDoCollisionTest = false;			// Should not zoom closer to player when colliding

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeRotation(FRotator(0.f, 15.f, 0.f));	//Does not work correctly - set in editor

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackCollider->InitBoxExtent(FVector(15.f, 15.f, 15.f));
	//OurAttack->SetupAttachment(RootComponent);
	AttackCollider->SetGenerateOverlapEvents(false);
	AttackCollider->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightHandSocket"));

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlap);

	//Using this for run speed. Have to set it to other than default 600 = walkspeed
	GetCharacterMovement()->MaxCustomMovementSpeed = 1000.f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 60.f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;	//has to be set to get crouch to work

	//Fetching the CharacterMovement so it stays in sync with what is set there
	//Safer to do in BeginPlay, because it then would get changes made in editor
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	MaxRunSpeed = GetCharacterMovement()->MaxCustomMovementSpeed;	//if doing this we don't need UProperties on these variables

	//Just debugging strange behaviour
	// if (AttackCollider->GetGenerateOverlapEvents() == true)
	// {
		// AttackCollider->SetGenerateOverlapEvents(false);
		// UE_LOG(LogTemp, Error, TEXT("Attack Collider GenerateOverlapEvents was set to true !"))
	// }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("isAttacking %s"), (AttackCollider->GetGenerateOverlapEvents() ? TEXT("True") : TEXT("False")))
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::StartCrouch);	//not allowed to call Crouch directly
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::StopCrouch);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::StopRun);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainCharacter::StopAttack);
	
	PlayerInputComponent->BindAction("SwitchInput", IE_Pressed, this, &AMainCharacter::SwitchInputType);
}

void AMainCharacter::MoveForward(float Value)
{
	FVector Direction;
	if (bNormalInputType)
	{
		// get forward vector
		 Direction = GetActorForwardVector();
	}
	else
	{
		FRotator tempRotation = CameraBoom->GetComponentRotation();
		tempRotation -= GetViewRotation();
		tempRotation.Pitch = 0.f;		// we don't want the pitch, only yaw
		Direction = tempRotation.Vector();
	}
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	FVector Direction;
	if (bNormalInputType)
	{
		// get forward vector
		Direction = GetActorRightVector();
	}
	else
	{
		FRotator tempRotation = CameraBoom->GetComponentRotation();
		tempRotation -= GetViewRotation();
		tempRotation.Pitch = 0.f;		// we don't want the pitch, only yaw
		tempRotation.Yaw += 90.f;
		Direction = tempRotation.Vector();
	}
	AddMovementInput(Direction, Value);
}

void AMainCharacter::StartCrouch()
{
	Crouch();		//from CharacterMovement
}

void AMainCharacter::StopCrouch()
{
	UnCrouch();		//from CharacterMovement
}

void AMainCharacter::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;		//Change to RunSpeed
}

void AMainCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;	//Change back to WalkSpeed
}

void AMainCharacter::StartAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Attack called"))

	//this is set in Blueprints by Anim Notifiers
	// if (AttackCollider)	//for some reason this is sometimes a nullptr at start
		// AttackCollider->SetGenerateOverlapEvents(true);
	bIsAttacking = true;	
}

void AMainCharacter::StopAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Attack called"))
	// bIsAttacking = false;
	bAttackFinished = true;		//wait for animation to fínish
	
	// this is set in Blueprints by Anim Notifiers
	// AttackCollider->SetGenerateOverlapEvents(false);
}

void AMainCharacter::AttackFinished()
{
	if(!bAttackFinished)
		StartAttack();
	else
	{
		bAttackFinished = false;
		bIsAttacking = false;
		UE_LOG(LogTemp, Warning, TEXT("Attack Finished called"))
	}
}	

void AMainCharacter::SwitchInputType()
{
	//toggle the input type
	bNormalInputType = !bNormalInputType;
}

// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Components/UPrimitiveComponent/OnComponentBeginOverlap/index.html
void AMainCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Overlaps %s"), *OtherActor->GetName())

	if (OtherActor->IsA(ADestructableBox::StaticClass()))
	{
		Cast<ADestructableBox>(OtherActor)->ImHit();
	}
}

