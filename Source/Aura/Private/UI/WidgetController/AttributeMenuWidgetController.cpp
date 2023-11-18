// Copyright Stas Company


#include "..\..\..\Public\UI\WidgetController\AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	check(auraAttributeSet);

	FAuraAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	info.AttributeValue = auraAttributeSet->GetStrength();
	AttributeInfoDelegate.Broadcast(info);
	
	OnArmorChanged.Broadcast(auraAttributeSet->GetArmor());
	OnMaxHealthChanged.Broadcast(auraAttributeSet->GetMaxHealth());
	OnMaxManaChanged.Broadcast(auraAttributeSet->GetMaxMana());
	OnArmorPenetrationChanged.Broadcast(auraAttributeSet->GetArmorPenetration());
	OnBlockChanceChanged.Broadcast(auraAttributeSet->GetBlockChance());
	OnCriticalHitChanceChanged.Broadcast(auraAttributeSet->GetCriticalHitChance());
	OnCriticalHitDamageChanged.Broadcast(auraAttributeSet->GetCriticalHitDamage());
	OnCriticalHitResistanceChanged.Broadcast(auraAttributeSet->GetCriticalHitResistance());
	OnHealthRegenerationChanged.Broadcast(auraAttributeSet->GetHealthRegeneration());
	OnManaRegenerationChanged.Broadcast(auraAttributeSet->GetManaRegeneration());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const auto auraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetArmorAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnArmorChanged.Broadcast(Data.NewValue);
	                      });

	// OnMaxHealthChanged.Broadcast(auraAttributeSet->GetMaxHealth());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnMaxHealthChanged.Broadcast(Data.NewValue);
	                      });
	// OnMaxManaChanged.Broadcast(auraAttributeSet->GetMaxMana());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetMaxManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnMaxManaChanged.Broadcast(Data.NewValue);
	                      });
	// OnArmorPenetrationChanged.Broadcast(auraAttributeSet->GetArmorPenetration());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetArmorPenetrationAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnArmorPenetrationChanged.Broadcast(Data.NewValue);
	                      });
	// OnBlockChanceChanged.Broadcast(auraAttributeSet->GetBlockChance());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetBlockChanceAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnBlockChanceChanged.Broadcast(Data.NewValue);
	                      });
	// OnCriticalHitChanceChanged.Broadcast(auraAttributeSet->GetCriticalHitChance());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetCriticalHitChanceAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnCriticalHitChanceChanged.Broadcast(Data.NewValue);
	                      });
	// OnCriticalHitDamageChanged.Broadcast(auraAttributeSet->GetCriticalHitDamage());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetCriticalHitDamageAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnCriticalHitDamageChanged.Broadcast(Data.NewValue);
	                      });
	// OnCriticalHitResistanceChanged.Broadcast(auraAttributeSet->GetCriticalHitResistance());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetCriticalHitResistanceAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnCriticalHitResistanceChanged.Broadcast(Data.NewValue);
	                      });
	// OnHealthRegenerationChanged.Broadcast(auraAttributeSet->GetHealthRegeneration());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetHealthRegenerationAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnHealthRegenerationChanged.Broadcast(Data.NewValue);
	                      });
	// OnManaRegenerationChanged.Broadcast(auraAttributeSet->GetManaRegeneration());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      auraAttributeSet->GetManaRegenerationAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnManaRegenerationChanged.Broadcast(Data.NewValue);
	                      });
}
