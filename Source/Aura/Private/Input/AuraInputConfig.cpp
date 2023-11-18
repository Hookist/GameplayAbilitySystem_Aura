// Copyright Stas Company


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FAuraInputAction& inputAction : AbilityInputActions)
	{
		if (inputAction.InputTag.MatchesTagExact(InputTag))
		{
			return inputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityInputAction was not found by tag [%s]"), *InputTag.ToString());
	}
	
	return nullptr;
}
