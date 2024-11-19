// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_BasePickup.h"
#include "CS_Character.h" 
#include "Components/SphereComponent.h"

// Sets default values
ACS_BasePickup::ACS_BasePickup()
{

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = SphereCollision;
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;

}

// Called when the game starts or when spawned
void ACS_BasePickup::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACS_BasePickup::OnBeginOverlap);	
}

// Handles the Character overlapping the pickup.
void ACS_BasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto Character = Cast<ACS_Character>(OtherActor))
	{
		Pickup(Character);
	}
}

// Called when the pickup is picked up.
void ACS_BasePickup::Pickup_Implementation(ACS_Character* OwningCharacter)
{
	SetOwner(OwningCharacter);
}


