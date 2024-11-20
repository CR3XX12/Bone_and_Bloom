#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CS_Interactable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCS_Interactable : public UInterface
{
	GENERATED_BODY()
};

class C280CHOMPERSTUDIOS_API ICS_Interactable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	void Interact(class ACS_Character* CharacterInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Can Interact"))
	bool CanInteract(ACS_Character* CharacterInstigator) const;
};
