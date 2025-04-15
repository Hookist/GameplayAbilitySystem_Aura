// Copyright Stas Company

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AuraGameStateBase.generated.h"

class ULevelUpInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameStateBase : public AGameState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	ULevelUpInfo* LevelUpInfo;

public:

	UFUNCTION(BlueprintCallable)
	ULevelUpInfo* GetLevelUpInfo() const; 
};
