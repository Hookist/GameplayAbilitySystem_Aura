// Copyright Stas Company


#include "UI/WidgetController/OverlayWidgetController.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(auraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(auraAttributeSet->GetMaxHealth());


}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetHealthAttribute())
		.AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxHealthAttribute())
		.AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
