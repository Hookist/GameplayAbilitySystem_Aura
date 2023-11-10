// Copyright Stas Company

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class UGameplayEffect;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraSpringArmComponent;

public:
	
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

#pragma region Combat Interface

	virtual int32 GetCreatureLevel() override;
	
#pragma endregion 

protected:

	virtual void InitAbilityActorInfo() override;
};