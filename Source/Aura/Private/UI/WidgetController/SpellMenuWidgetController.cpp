// Copyright Stas Company


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	BroadcastAbilityInfo();
	OnSpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
	//GetAuraPS()->GetSpellPoints()
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FAuraAbilityInfo info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(info);
		}
	});

	GetAuraPS()->OnSpellPointsChanged.AddLambda([this](int32 SpellPoints, int32 OldSpellPoints)
	{
		OnSpellPointsChanged.Broadcast(SpellPoints);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags gameplayTags = FAuraGameplayTags::Get();
	const int32 spellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag abilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(gameplayTags.Abilities_None);
	const FGameplayAbilitySpec* abilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = abilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		abilityStatus = gameplayTags.Abilities_Status_Locked;
	}
	else
	{
		abilityStatus = GetAuraASC()->GetStatusFromSpec(*abilitySpec);
	}

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(abilityStatus, spellPoints, bEnableSpendPoints, bEnableEquip);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
	bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags gameplayTags = FAuraGameplayTags::Get();

	bShouldEnableEquipButton = false;
	bShouldEnableSpellPointsButton = false;
	if (AbilityStatus.MatchesTagExact(gameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(gameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(gameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableSpellPointsButton = true;
		bShouldEnableEquipButton = true;
	}
}
