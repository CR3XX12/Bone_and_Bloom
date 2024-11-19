// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CS_Character.generated.h"

UCLASS()
class C280CHOMPERSTUDIOS_API ACS_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	/** Used as a main camera for the character: it is connected through a SpringArmComponent. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	/** The default input mapping context for the character: handles movement, look around and interaction. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	// Declare the basic input actions (movement, interaction, etc.)

	/** The input action for moving the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	/** The input action for looking around with the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** The input action for sprinting with the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	/** The input action for interacting with the environment. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* CharacterDataTable;

	/** The character stats, retrieved from the data table. */
	struct FCS_CharacterStats* CharacterStats;

public:
	// Sets default values for this character's properties
	ACS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Declare the basic actions (movement, interaction, etc.)

	/** The function called when the character moves. */
	void Move(const struct FInputActionValue& Value);

	/** The function called when the character looks around. */
	void Look(const FInputActionValue& Value);

	/** The function called when the character starts sprinting. */
	void SprintStart(const FInputActionValue& Value);

	/** The function called when the character stops sprinting. */
	void SprintEnd(const FInputActionValue& Value);


	/** The function called when the character interacts with the environment. */
	void Interact(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Updates the character stats based on the level. */
	void UpdateCharacterStats(int32 CharacterLevel);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns the CameraComponent used as a main camera for the character. */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Getter for the character statistics based on the level
	FORCEINLINE FCS_CharacterStats* GetCharacterStats() const { return CharacterStats; }

};
