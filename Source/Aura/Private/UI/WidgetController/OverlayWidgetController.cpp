// Copyright Stas Company


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInformation.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const auto auraAttributeSet = GetAuraAS();
	
	OnHealthChanged.Broadcast(auraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(auraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(auraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(auraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const auto auraPlayerState = GetAuraPS();
	const auto auraAttributeSet = GetAuraAS();
	
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

	if (auto auraASC = GetAuraASC())
	{
		if (auraASC->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			auraASC->AbilitiesGivenDelegate.AddUObject(this,  &ThisClass::BroadcastAbilityInfo);
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
		auraPlayerState->OnLevelChanged.AddLambda(
			[this](int32 NewLevel, int32 OldLevel)
			{
				OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
			}
		);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* InAuraAbilitySystemComponent)
{
	if (!GetAuraASC()->bStartupAbilitiesGiven) return;

	FForEachAbility broadcastDelegate;
	broadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo info = AbilityInfo->FindAbilityInfoByTag(GetAuraASC()->GetAbilityTagFromSpec(AbilitySpec));
		info.InputTag = GetAuraASC()->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(info);
	});
	GetAuraASC()->ForEachAbility(broadcastDelegate);
}

void UOverlayWidgetController::HandleXPChanged(int32 NewXP, int32 OldXP)
{
	const ULevelUpInfo* levelUpInfo = GetAuraPS()->LevelUpInfo;

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
