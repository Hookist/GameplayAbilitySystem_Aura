// Copyright Stas Company


#include <UI/WidgetController/AttributeMenuWidgetController.h>

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	check(auraAttributeSet);

	for (auto& Pair : auraAttributeSet->TagsToAttributes)
	{
		FAuraAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		info.AttributeValue = Pair.Value().GetNumericValue(auraAttributeSet);
		AttributeInfoDelegate.Broadcast(info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
