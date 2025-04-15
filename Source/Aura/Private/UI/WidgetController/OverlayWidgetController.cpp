// Copyright Stas Company


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInformation.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(auraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(auraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(auraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(auraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const auto auraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const auto auraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	if (auto auraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (auraASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(auraASC);
		}
		else
		{
			auraASC->AbilitiesGivenDelegate.AddUObject(this,  &ThisClass::OnInitializeStartupAbilities);
		}	
		
		auraASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& assetTags)
			{
				for (const FGameplayTag& tag : assetTags)
				{
					FGameplayTag messageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (tag.MatchesTag(messageTag))
					{
						const auto row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, tag);
						MessageWidgetRowDelegate.Broadcast(*row);
					}
				}
			}
		);
	}

	
	if (auraPlayerState)
	{
		auraPlayerState->OnXPChanged.AddUObject(this, &UOverlayWidgetController::HandleXPChanged);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	//TODO Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.

	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility broadcastDelegate;
	broadcastDelegate.BindLambda([this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		//TODO need a way to figure out the ability tag for a given ability spec.
		FAuraAbilityInfo info = AbilityInfo->FindAbilityInfoByTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(info);
	});
	AuraAbilitySystemComponent->ForEachAbility(broadcastDelegate);
}

void UOverlayWidgetController::HandleXPChanged(int32 NewXP, int32 OldXP) const
{
	const auto auraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* levelUpInfo = auraPlayerState->LevelUpInfo;

	checkf(levelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	const int32 level = levelUpInfo->FindLevelForXP(NewXP);
	const int32 maxLevel = levelUpInfo->LevelUpInformation.Num();
	
	if (level <= maxLevel && level > 0)
	{
		const int32 levelUpRequirement = levelUpInfo->LevelUpInformation[level].LevelUpRequirement;
		const int32 previousLevelUpRequirement = levelUpInfo->LevelUpInformation[level - 1].LevelUpRequirement;

		const int32 deltaLevelRequirement = levelUpRequirement - previousLevelUpRequirement;
		const int32 xpForThisLevel = NewXP - previousLevelUpRequirement;

		const float xpBarPercent = static_cast<float>(xpForThisLevel) / static_cast<float>(deltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(xpBarPercent);
	}
}
