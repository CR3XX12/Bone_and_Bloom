// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_PlayerState.h"

#include "CS_Character.h" 
#include "CS_CharacterStats.h" 
#include "Net/UnrealNetwork.h"

void ACS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACS_PlayerState, Xp, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACS_PlayerState, CharacterLevel, COND_OwnerOnly);
}

void ACS_PlayerState::AddXp(const int32 Value)
{
	Xp += Value;
	OnXpChanged.Broadcast(Xp);

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("Total Xp: %d"), Value));

	if (const auto Character = Cast<ACS_Character>(GetPawn()))
	{
		if (Character->GetCharacterStats()->NextLevelXp < Xp)
		{
			GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Level Up!"));

			CharacterLevel++;
			Character->UpdateCharacterStats(CharacterLevel);
			OnCharacterLevelUp.Broadcast(CharacterLevel);
		}
	}
}

void ACS_PlayerState::OnRep_Xp(int32 OldValue) const
{
	OnXpChanged.Broadcast(Xp);
}

void ACS_PlayerState::OnRep_CharacterLevelUp(int32 OldValue) const
{
	OnCharacterLevelUp.Broadcast(CharacterLevel);
}

/**
 * Called to get the list of replicated properties
 * @param OutLifetimeProps - List of replicated properties
 */