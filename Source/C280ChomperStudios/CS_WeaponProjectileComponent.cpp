// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_WeaponProjectileComponent.h"
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h" 
#include "CS_BaseWeaponProjectile.h" 
#include "CS_Character.h"

// Sets default values for this component's properties
UCS_WeaponProjectileComponent::UCS_WeaponProjectileComponent()
{
	ProjectileClass = ACS_BaseWeaponProjectile::StaticClass();
}


// Called when the game starts
void UCS_WeaponProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// Add the base mapping context to the player controller only if we are using a PlayerController
	const ACS_Character* Character = Cast<ACS_Character>(GetOwner());
	if(!Character) return;
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WeaponMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &UCS_WeaponProjectileComponent::Throw);
		}
	}
}

void UCS_WeaponProjectileComponent::Throw()
{
	Throw_Server();
}

void UCS_WeaponProjectileComponent::Throw_Server_Implementation()
{
	// Spawn the projectile, setting its owner and instigator as the spawning character
	if (ProjectileClass)
	{
		// Call the client side method to play the animation
		Throw_Client();
		// Delay the spawn of the projectile to match the animation
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			const auto Character = Cast<ACS_Character>(GetOwner());
			const auto ProjectileSpawnLocation = GetComponentLocation();
			const auto ProjectileSpawnRotation = GetComponentRotation();
			auto ProjectileSpawnParams = FActorSpawnParameters();
			ProjectileSpawnParams.Owner = GetOwner();
			ProjectileSpawnParams.Instigator = Character;

			GetWorld()->SpawnActor<ACS_BaseWeaponProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation, ProjectileSpawnParams);
		}, .5f, false);
	}
}

void  UCS_WeaponProjectileComponent::Throw_Client_Implementation()
{
	// Play the animation on the client side
	const auto Character = Cast<ACS_Character>(GetOwner());
	if (ThrowAnimation != nullptr)
	{
		if (const auto AnimInstance = Character->GetMesh()->GetAnimInstance(); AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(ThrowAnimation, 1.f);
		}
	}	
}

void UCS_WeaponProjectileComponent::SetProjectileClass(TSubclassOf<ACS_BaseWeaponProjectile> NewProjectileClass)
{
	
	ProjectileClass = NewProjectileClass;
}

