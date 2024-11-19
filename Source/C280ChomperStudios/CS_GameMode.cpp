// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_GameMode.h"
#include "CS_GameState.h" 
#include "CS_PlayerController.h" 
#include "CS_PlayerState.h" 
#include "CS_Character.h" 
#include "UObject/ConstructorHelpers.h"


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
