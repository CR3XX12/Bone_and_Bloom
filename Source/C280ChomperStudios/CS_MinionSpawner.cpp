// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_MinionSpawner.h"
#include "CS_Minion.h" 
#include "Components/BoxComponent.h"

// Sets default values
ACS_MinionSpawner::ACS_MinionSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));

}

// Called when the game starts or when spawned
void ACS_MinionSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnableMinions.IsEmpty()) return;
	if (GetLocalRole() != ROLE_Authority) return;

	// Spawn NumMinionsAtStart minions at the start of the game
	for (int32 i = 0; i < NumMinionsAtStart; i++)
	{
		Spawn();
	}

	// Call Spawn() every SpawnDelay seconds
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ACS_MinionSpawner::Spawn, SpawnDelay, true, SpawnDelay);

	
}

void ACS_MinionSpawner::Spawn()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	const auto Minion =
		SpawnableMinions[FMath::RandRange(0, SpawnableMinions.Num() - 1)];

	const auto Rotation =
		FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
	const auto Location =
		SpawnArea->GetComponentLocation() +
		FVector(
			FMath::RandRange(-SpawnArea->GetScaledBoxExtent().X, SpawnArea->GetScaledBoxExtent().X),
			FMath::RandRange(-SpawnArea->GetScaledBoxExtent().Y, SpawnArea->GetScaledBoxExtent().Y),
			0.0f);

	GetWorld()->SpawnActor<ACS_Minion>(Minion, Location, Rotation, SpawnParams);
}

// Called every frame
void ACS_MinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

