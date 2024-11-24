// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class C280CHOMPERSTUDIOS_API ACS_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACS_GameMode();
	UFUNCTION(BlueprintCallable, Category = "Minions")
	void AlertMinions(class AActor* AlertInstigator, const FVector& Location, float Radius);

	
};
