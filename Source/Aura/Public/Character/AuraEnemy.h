// Copyright Stas Company

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interactable.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractable
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;

public:

	AAuraEnemy();

	virtual void BeginPlay() override;

#pragma region Combat Interface

	virtual int32 GetCreatureLevel() override;
	
#pragma endregion

#pragma region Interactable
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void BP_HighlightActor();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_UnHighlightActor();

	virtual void InitAbilityActorInfo() override;

#pragma endregion

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	
};
