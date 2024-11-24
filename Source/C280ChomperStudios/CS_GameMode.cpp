// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_GameMode.h"
#include "CS_GameState.h" 
#include "CS_PlayerController.h" 
#include "CS_PlayerState.h" 
#include "CS_Character.h" 
#include "UObject/ConstructorHelpers.h"
#include "CS_Minion.h" 
#include "Kismet/GameplayStatics.h"


ACS_GameMode::ACS_GameMode()
{
	GameStateClass = ACS_GameState::StaticClass();
	PlayerStateClass = ACS_PlayerState::StaticClass();
	PlayerControllerClass = ACS_PlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void ACS_GameMode::AlertMinions(AActor* AlertInstigator, const FVector& Location, const float Radius)
{
	TArray<AActor*> Minions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACS_Minion::StaticClass(), Minions);
	for (const auto Minion : Minions)
	{
		if (AlertInstigator == Minion) continue;
		if (const auto Distance = FVector::Distance(AlertInstigator->GetActorLocation(), Minion->GetActorLocation()); Distance < Radius)
		{
			if (const auto MinionCharacter = Cast<ACS_Minion>(Minion))
			{
				MinionCharacter->GoToLocation(Location);
			}
		}
	}
}

