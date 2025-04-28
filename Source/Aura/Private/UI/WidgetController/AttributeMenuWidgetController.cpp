// Copyright Stas Company


#include <UI/WidgetController/AttributeMenuWidgetController.h>

#include "AbilitySystem/Data/AttributeInfo.h"
#include "Chaos/Pair.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	check(auraAttributeSet);

	for (auto& Pair : auraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	const auto auraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	OnPlayerAttributePointsChangedDelegate.Broadcast(auraPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	check(auraAttributeSet);
	
	for (auto& Pair : auraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	const auto auraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);

	if (auraPlayerState)
	{
		auraPlayerState->OnAttributePointsChanged.AddLambda(
			[this](int32 NewPoints, int32 OldPoints)
			{
				OnPlayerAttributePointsChangedDelegate.Broadcast(NewPoints);
			}
		);

		auraPlayerState->OnSpellPointsChanged.AddLambda(
			[this](int32 NewPoints, int32 OldPoints)
			{
				OnPlayerSpellPointsChangedDelegate.Broadcast(NewPoints);
			}
		);
	}

}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(info);
}
