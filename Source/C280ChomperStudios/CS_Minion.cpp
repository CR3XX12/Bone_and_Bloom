// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_Minion.h"
#include "AIController.h" 
#include "NavigationSystem.h" 
#include "CS_Character.h" 
#include "Components/CapsuleComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h" 
#include "Blueprint/AIBlueprintHelperLibrary.h" 
#include "Components/SphereComponent.h"
//#include "US_GameMode.h"
//#include "US_BasePickup.h"

// Sets default values
ACS_Minion::ACS_Minion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SensingInterval = .8f;
	PawnSense->SetPeripheralVisionAngle(45.f);
	PawnSense->SightRadius = 1500.f;
	PawnSense->HearingThreshold = 400.f;
	PawnSense->LOSHearingThreshold = 800.f;
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(100);
	Collision->SetupAttachment(RootComponent);
	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMeshAsset(TEXT("/Game/KayKit/Skeletons/skeleton_minion"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

}

// Called when the game starts or when spawned
void ACS_Minion::BeginPlay()
{
	Super::BeginPlay();
	SetNextPatrolLocation();
}

void ACS_Minion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetLocalRole() != ROLE_Authority) return;
	//OnTakeAnyDamage.AddDynamic(this, &ACS_Minion::OnDamage);
	OnActorBeginOverlap.AddDynamic(this, &ACS_Minion::OnBeginOverlap);
	GetPawnSense()->OnSeePawn.AddDynamic(this, &ACS_Minion::OnPawnDetected);

	//GetPawnSense()->OnHearNoise.AddDynamic(this, &AUS_Minion::OnHearNoise);

}

void ACS_Minion::OnPawnDetected(APawn* Pawn)
{
	if (!Pawn->IsA<ACS_Character>()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Character detected!"));
	if (GetCharacterMovement()->MaxWalkSpeed != ChaseSpeed)
	{
		Chase(Pawn);
	}
}

void ACS_Minion::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->IsA<ACS_Character>()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Character captured!"));
}

void ACS_Minion::SetNextPatrolLocation()
{
	if (GetLocalRole() != ROLE_Authority) return;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	const auto LocationFound = UNavigationSystemV1::K2_GetRandomReachablePointInRadius(
		this, GetActorLocation(), PatrolLocation, PatrolRadius);
	if (LocationFound)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PatrolLocation);
	}
}
void ACS_Minion::Chase(APawn* Pawn)
{
	if (GetLocalRole() != ROLE_Authority) return;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Pawn);
	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 25.f, 12, FColor::Red, true, 10.f, 0, 2.f);

	/*if (const auto GameMode = Cast<ACS_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->AlertMinions(this, Pawn->GetActorLocation(), AlertRadius);
	}*/
}

// Called every frame
void ACS_Minion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority) return;

	if (GetMovementComponent()->GetMaxSpeed() == ChaseSpeed) return;

	if ((GetActorLocation() - PatrolLocation).Size() < 500.f)
	{
		SetNextPatrolLocation();
	}

}

// Called to bind functionality to input
void ACS_Minion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

