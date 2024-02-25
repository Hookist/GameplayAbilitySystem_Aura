// Copyright Stas Company


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{

	GameplayTags.Attributes_Primary_Strength =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),
	FString("Increase dmg"));

	GameplayTags.Attributes_Primary_Intelligence =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),
	FString("Increase magical dmg"));

	GameplayTags.Attributes_Primary_Resilience =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),
FString("Increase armor and armor penetration"));

	GameplayTags.Attributes_Primary_Vigor =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),
	FString("Increase max health and regen"));
	
	GameplayTags.Attributes_Secondary_Armor =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance"));

	GameplayTags.Attributes_Secondary_MaxHealth =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),
	FString("more health = more chance to survive"));

	GameplayTags.Attributes_Secondary_MaxMana =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),
FString("more mana = more spells"));

	GameplayTags.Attributes_Secondary_ArmorPenetration =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),
FString("increase chance to ignore armor"));

	GameplayTags.Attributes_Secondary_BlockChance =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),
FString("increase chance to block attack"));

	GameplayTags.Attributes_Secondary_CriticalHitChance =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),
FString("chance to dd"));

	GameplayTags.Attributes_Secondary_CriticalHitDamage =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),
FString("crit additional damage"));

	GameplayTags.Attributes_Secondary_CriticalHitResistance =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),
FString("chance avoid crit"));

	GameplayTags.Attributes_Secondary_HealthRegeneration =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),
FString("health restoring speed per second"));

	GameplayTags.Attributes_Secondary_ManaRegeneration =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),
FString("mana restoring speed per second"));

	GameplayTags.Attributes_Vital_Health =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"),
FString("health"));

	GameplayTags.Attributes_Vital_Mana =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"),
FString("mana"));

	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"),
		                                                 FString("Input Tag for left mouse button"));

	GameplayTags.InputTag_RMB =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"),
													 FString("Input Tag for right mouse button"));
	GameplayTags.InputTag_1 =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"),
													 FString("Input Tag for button 1"));
	GameplayTags.InputTag_2 =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"),
												 FString("Input Tag for button 2"));
	GameplayTags.InputTag_3 =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"),
												 FString("Input Tag for button 3"));
	GameplayTags.InputTag_4 =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"),
												 FString("Input Tag for button 4"));

	GameplayTags.Damage =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),
											 FString("Damage"));

	/*
	* Effects
	*/

	GameplayTags.Effects_HitReact =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),
										 FString("Hit react effect"));
}
