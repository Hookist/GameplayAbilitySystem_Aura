// Copyright Stas Company

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interactable.h"
#include "AuraEnemy.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractable
{
	GENERATED_BODY()

public:

	AAuraEnemy();

	virtual void BeginPlay() override;

#pragma region Interactable
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void BP_HighlightActor();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_UnHighlightActor();

#pragma endregion
};
