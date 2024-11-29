#include "CS_BaseWeaponProjectile.h"
#include "CS_Character.h" 
#include "CS_CharacterStats.h" 
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h" 
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACS_BaseWeaponProjectile::ACS_BaseWeaponProjectile()
{
    // Initialize the collision sphere
    SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
    SphereCollision->SetGenerateOverlapEvents(true);
    SphereCollision->SetSphereRadius(10.0f);
    SphereCollision->BodyInstance.SetCollisionProfileName("BlockAll");
    SphereCollision->OnComponentHit.AddDynamic(this, &ACS_BaseWeaponProjectile::OnHit);

    RootComponent = SphereCollision;

    // Initialize the mesh
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    Mesh->SetRelativeLocation(FVector(-40.f, 0.f, 0.f));
    Mesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/KayKit/DungeonElements/dagger_common"));
    if (StaticMesh.Succeeded())
    {
        GetMesh()->SetStaticMesh(StaticMesh.Object);
    }

    // Initialize the projectile movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    ProjectileMovement->UpdatedComponent = SphereCollision;
    ProjectileMovement->ProjectileGravityScale = 0;
    ProjectileMovement->InitialSpeed = 3000;
    ProjectileMovement->MaxSpeed = 3000;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    // Make the projectile replicate
    bReplicates = true;
}

// Called when the projectile is spawned
void ACS_BaseWeaponProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // Ignore collision with the instigator
    if (GetInstigator())
    {
        SphereCollision->IgnoreActorWhenMoving(GetInstigator(), true);
    }
}

// Called when the projectile hits another object
void ACS_BaseWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Compute the damage based on the character stats
    auto ComputedDamage = Damage;
    if (const auto Character = Cast<ACS_Character>(GetInstigator()))
    {
        ComputedDamage *= Character->GetCharacterStats()->DamageMultiplier;
    }

    // Apply the damage to the actor that was hit
    if (OtherActor && OtherActor != this)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Applying damage to actor: %s"), *OtherActor->GetName()));
        }

        const FDamageEvent Event(UDamageType::StaticClass());
        OtherActor->TakeDamage(ComputedDamage, Event, GetInstigatorController(), this);
    }

    // Destroy the projectile
    Destroy();
}
