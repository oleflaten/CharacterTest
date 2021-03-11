// OEF 2021


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DestructableBox.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom - the placement/position of the 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;	//the distance from the camera down to the player
	CameraBoom->bEnableCameraLag = true;	//just to let the camera lag and not be that fixed at the end of the boom
	CameraBoom->CameraLagSpeed = 40.f;		//how much lag
	CameraBoom->SetRelativeRotation(FRotator(-40.f, 80.f, 0.f)); //X = pitch = angle down on player, Y = yaw = angle around the player
	CameraBoom->bInheritYaw = false;				// Ignore Yaw rotations - since we want a fixed camera
	CameraBoom->bDoCollisionTest = false;			// Should not zoom closer to player when colliding  - since we want a fixed camera

	// Create a follow camera - the actual camera the game is viewed through
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	//put it at the end of the boom

	//Simple melee attack box - this should eventually be put on a socket on the skeleton of the player to follow the attack-animation
	OurAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerAttack"));
	OurAttack->InitBoxExtent(FVector(30.f, 30.f, 30.f));
	OurAttack->SetupAttachment(RootComponent);
	OurAttack->SetGenerateOverlapEvents(false);			//do not make overlap events when game starts
	OurAttack->SetRelativeLocation(AttackPlacement);	//placement relative to the root of this pawn

	//Check if we hit anything when we attack
	OurAttack->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlap);

	//Using this for run speed. Have to set it to other than default 600 = walkspeed
	//MovementComponent does not have a dedicated RunSpeed variable
	GetCharacterMovement()->MaxCustomMovementSpeed = 1000.f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 60.f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;	//has to be set to get crouch to work

	//Fetching the CharacterMovement so it stays in sync with what is set there
	//Safer to do in BeginPlay, because it then would get changes made in editor
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	MaxRunSpeed = GetCharacterMovement()->MaxCustomMovementSpeed;	//if doing this we don't need UProperties on these variables
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Help-functionallity for smashing - only needed because we have no attack-animation
	//A stand still hitbox does not trigger anything.
	if (isSmashing)
	{
		//Move the hitbox to trigger the overlap event
		OurAttack->SetRelativeLocation(AttackPlacement + temp);
		temp *= -1.f;	
	}
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
	PlayerInputComponent->BindAction("Smash", IE_Pressed, this, &AMainCharacter::StartSmash);
	PlayerInputComponent->BindAction("Smash", IE_Released, this, &AMainCharacter::StopSmash);
	
	PlayerInputComponent->BindAction("SwitchInput", IE_Pressed, this, &AMainCharacter::SwitchInputType);
}

void AMainCharacter::MoveForward(float Value)
{
	FVector Direction;
	if (normalInputType)
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
	if (normalInputType)
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
	GetMesh()->SetRelativeScale3D(FVector(1.f, 1.f, 0.6f));		//before we have animations
}

void AMainCharacter::StopCrouch()
{
	UnCrouch();		//from CharacterMovement
	GetMesh()->SetRelativeScale3D(FVector(1.f));				//before we have animations
}

void AMainCharacter::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;		//Change to RunSpeed
}

void AMainCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;	//Change back to WalkSpeed
}

void AMainCharacter::StartSmash()
{
	UE_LOG(LogTemp, Warning, TEXT("Smash!"));
	OurAttack->SetGenerateOverlapEvents(true);
	isSmashing = true;	//only needed until we get animation
}

void AMainCharacter::StopSmash()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Smash!"));
	OurAttack->SetGenerateOverlapEvents(false);
	isSmashing = false;	//only needed until we get animation
}

void AMainCharacter::SwitchInputType()
{
	//toggle the input type
	normalInputType = !normalInputType;
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

