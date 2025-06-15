// Copyright Stas Company


#include "UI/WidgetController/SpellMenuWidgetController.h"

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
