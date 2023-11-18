// Copyright Stas Company


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> abilityClass : StartupAbilities)
	{
		auto abilitySpec = FGameplayAbilitySpec(abilityClass, 1);
		//GiveAbility(abilitySpec);
		GiveAbilityAndActivateOnce(abilitySpec);
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer);

	EffectAssetTags.Broadcast(tagContainer);
}
