// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CS_BasePickup.generated.h"

UCLASS()
class C280CHOMPERSTUDIOS_API ACS_BasePickup : public AActor
{
	GENERATED_BODY()

	/** The collision component used to detect the character overlapping the pickup. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;

	/** The static mesh used to represent the pickup. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;
	
public:	
	// Sets default values for this actor's properties
	ACS_BasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** The function called when the character overlaps the pickup. */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** The function called when the pickup is picked up. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup", meta = (DisplayName = "Pickup"))
	void Pickup(class ACS_Character* OwningCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/** Returns the BoxCollision used as a trigger for the character. */
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }

	/** Returns the Static Mesh. */
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

};
