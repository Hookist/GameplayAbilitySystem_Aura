// Copyright Stas Company


#include <UI/WidgetController/AttributeMenuWidgetController.h>

#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	check(auraAttributeSet);

	for (auto& Pair : auraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
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
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(info);
}
