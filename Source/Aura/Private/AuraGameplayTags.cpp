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
	 *	Damage Types
	 */
	
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),
	FString("Fire damage type"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"),
	FString("Lightning damage type"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"),
	FString("Arcane damage type"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),
	FString("Physical damage type"));

	/*
	 *	Resistances
	 */

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),
	FString("Resistance to Fire damage"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),
	FString("Resistance to Arcane damage"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),
	FString("Resistance to Lightning damage"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),
	FString("Resistance to Physical damage"));

	/*
	 * Map of Damage Types to Resistances
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	
	/*
	 * Effects
	 */

	GameplayTags.Effects_HitReact =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),
										 FString("Hit react effect"));

	/*
	 * Abilities
	 */

	GameplayTags.Abilities_Attack =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"),
										 FString("Attack ability Tag"));

	/*
	 * Combat sockets
	 */

	GameplayTags.CombatSocket_Weapon =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"),
		FString("Weapon"));

	GameplayTags.CombatSocket_RightHand =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"),
	FString("Right Hand"));
	
	GameplayTags.CombatSocket_LeftHand =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"),
	FString("Left Hand"));

	GameplayTags.CombatSocket_Tail =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"),
FString("Tail"));
	
	/*
	 * Montage tags
	 */

	GameplayTags.Montage_Attack_1 =
		UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"),
		FString("Attack 1"));
	GameplayTags.Montage_Attack_2 =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"),
	FString("Attack 2"));
	
	GameplayTags.Montage_Attack_3 =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"),
	FString("Attack 3"));
	
	GameplayTags.Montage_Attack_4 =
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"),
	FString("Attack 4"));
}
