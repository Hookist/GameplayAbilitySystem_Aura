// Copyright Stas Company

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStatChanged, int32 /*New Value*/ , int32 /*Old Value*/);

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	FPlayerStatChanged OnXPChanged;

	FPlayerStatChanged OnLevelChanged;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintCallable)
	void SetLevel(int32 InLevel);

	UFUNCTION(BlueprintCallable)
	void AddToLevel(int32 InLevel);
	
	FORCEINLINE int32 GetCreatureLevel() const { return Level; }

	UFUNCTION(BlueprintCallable)
	void SetXP(int32 NewXP);

	UFUNCTION(BlueprintCallable)
	void AddToXP(int32 XPToAdd);

	UFUNCTION(BlueprintCallable)
	int32 GetXP() const;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UFUNCTION()
	void OnRep_XP(int32 OldXP);
};
