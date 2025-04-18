// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CS_CharacterStats.h" 
#include "Engine/DataTable.h"
#include "CS_Interactable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "CS_WeaponProjectileComponent.h"



// Sets default values
ACS_Character::ACS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create the follow camera and attach it to the camera boom
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create the noise emitter
	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
	NoiseEmitter->NoiseLifetime = 0.01f;

	Weapon = CreateDefaultSubobject<UCS_WeaponProjectileComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetRelativeLocation(FVector(120.f, 70.f, 0.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);

	// Set the skeletal mesh for the character
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}

	// Set the character movement properties
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Adjustments for walking on convex colliders like stairs
	GetCharacterMovement()->SetWalkableFloorAngle(60.0f); // Allow steeper angles
	GetCharacterMovement()->MaxStepHeight = 45.0f;       // Increase step height
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Ensure proper collision

}



// Called when the game starts or when spawned
void ACS_Character::BeginPlay()
{
	Super::BeginPlay();

	// Add the base mapping context to the player controller only if we are using a PlayerController
	// and if the subsystem is available
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	UpdateCharacterStats(1);
}

// Called every frame
void ACS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority) return;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);

	auto SphereRadius = 50.f;
	auto StartLocation = GetActorLocation() + GetActorForwardVector() * 150.f;
	auto EndLocation = StartLocation + GetActorForwardVector() * 500.f;

	auto IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);
	// Check if the hit result implements the US_Interactable interface
	if (IsHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UCS_Interactable::StaticClass()))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, FColor::Magenta, false, 1.f);
		InteractableActor = HitResult.GetActor();
	}
	else
	{
		InteractableActor = nullptr;
	}

	// If the character is running, emit noise
	if (GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed)
	{
		auto Noise = 1.f;
		if (GetCharacterStats() && GetCharacterStats()->StealthMultiplier)
		{
			Noise = Noise / GetCharacterStats()->StealthMultiplier;
		}
		NoiseEmitter->MakeNoise(this, Noise, GetActorLocation());
	}

}

// Called to bind functionality to input
void ACS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the each input action to the corresponding methods
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACS_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACS_Character::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACS_Character::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACS_Character::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACS_Character::SprintEnd);
	}

}

void ACS_Character::Move(const FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2D>();
	//	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("MovementVector: %s"), *MovementVector.ToString()));

		// Only add movement if there is a controller possessing this actor
	if (Controller != nullptr)
	{
		const auto Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACS_Character::Look(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<FVector2D>();
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("LookAxisVector: %s"), *LookAxisVector.ToString()));

		// Only add look around if there is a controller possessing this actor
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Handle the change of speed when the sprint button is pressed
void ACS_Character::SprintStart(const FInputActionValue& Value)
{
	SprintStart_Server();
}

// Handle the change of speed when the sprint button is released
void ACS_Character::SprintEnd(const FInputActionValue& Value)
{
	SprintEnd_Server();
}

// Executes the sprint start action from the server
void ACS_Character::SprintStart_Server_Implementation()
{
	SprintStart_Client();
}

// Executes the sprint end action from the server
void ACS_Character::SprintEnd_Server_Implementation()
{
	SprintEnd_Client();
}

// Client-side implementation for starting the sprint
void ACS_Character::SprintStart_Client_Implementation()
{
	if (GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->SprintSpeed;
	}
}

// Client-side implementation for ending the sprint
void ACS_Character::SprintEnd_Client_Implementation()
{
	if (GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
	}
}

void ACS_Character::Interact(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(30, 5.f, FColor::Red, TEXT("Interact"));
	Interact_Server();
	
}

void ACS_Character::Interact_Server_Implementation()
{
	if (InteractableActor)
	{
		ICS_Interactable::Execute_Interact(InteractableActor, this);
	}
}

void ACS_Character::UpdateCharacterStats(int32 CharacterLevel)
{
		auto IsSprinting = false;
		if (GetCharacterStats())
		{
			IsSprinting = GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed;
		}
		if (CharacterDataTable)
		{
		TArray<FCS_CharacterStats*> CharacterStatsRows;
		CharacterDataTable->GetAllRows<FCS_CharacterStats>(TEXT("CS_Character"), CharacterStatsRows);
		
		if (CharacterStatsRows.Num() > 0)
		{
		const auto NewCharacterLevel = FMath::Clamp(CharacterLevel, 1, CharacterStatsRows.Num());
		CharacterStats = CharacterStatsRows[NewCharacterLevel - 1];
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
			if (IsSprinting)
			{
				SprintStart_Server();
			}
		}
	}
}

