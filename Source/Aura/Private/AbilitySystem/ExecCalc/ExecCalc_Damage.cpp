// Copyright Stas Company


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLightning);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceArcane);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceFire, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceLightning, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceArcane, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistancePhysical, Target, false);

		const FAuraGameplayTags& tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		
		TagsToCaptureDefs.Add(tags.Attributes_Resistance_Fire, ResistanceFireDef);
		TagsToCaptureDefs.Add(tags.Attributes_Resistance_Lightning, ResistanceLightningDef);
		TagsToCaptureDefs.Add(tags.Attributes_Resistance_Arcane, ResistanceArcaneDef);
		TagsToCaptureDefs.Add(tags.Attributes_Resistance_Physical, ResistancePhysicalDef);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().ResistanceLightningDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceArcaneDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistanceFireDef);
	RelevantAttributesToCapture.Add(DamageStatics().ResistancePhysicalDef);
}


void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* sourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* targetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* sourceAvatar = sourceASC ? sourceASC->GetAvatarActor() : nullptr;
	AActor* targetAvatar = targetASC ? targetASC->GetAvatarActor() : nullptr;
	ICombatInterface* sourceCombatInterface = Cast<ICombatInterface>(sourceAvatar);
	ICombatInterface* targetCombatInterface = Cast<ICombatInterface>(targetAvatar);

	const FGameplayEffectSpec& spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters evaluationParameters;
	evaluationParameters.SourceTags = sourceTags;
	evaluationParameters.TargetTags = targetTags;

	const UCharacterClassInfo* characterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(sourceAvatar);

	// Get Damage set by Caller Magnitude
	float damage = 0.f;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag& damageTypeTag = Pair.Key;
		const FGameplayTag& resistanceTag = Pair.Value;
		
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(resistanceTag),
			TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *resistanceTag.ToString());

		const FGameplayEffectAttributeCaptureDefinition captureDef = AuraDamageStatics().TagsToCaptureDefs[resistanceTag];

		float damageTypeValue = spec.GetSetByCallerMagnitude(damageTypeTag);
		
		float resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evaluationParameters, resistance);
		resistance = FMath::Clamp(resistance, 0.f, 100.f);

		damageTypeValue *= (100.f - resistance) / 100.f;
		
		damage += damageTypeValue;
	}
	
	// Capture BlockChance on Target, and determine if there was a successful Block

	float targetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, evaluationParameters, targetBlockChance);
	targetBlockChance = FMath::Max<float>(0.f, targetBlockChance);
	
	const bool bBlocked = FMath::RandRange(1, 100) <= targetBlockChance;

	FGameplayEffectContextHandle effectContextHandle = spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(effectContextHandle, bBlocked);
	
	// if Block, halve the damage.
	damage = bBlocked ? UKismetMathLibrary::SafeDivide(damage, 2.f) : damage;
	
	float targetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, evaluationParameters, targetArmor);
	targetArmor = FMath::Max<float>(0.f, targetArmor);

	float sourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, evaluationParameters, sourceArmorPenetration);
	sourceArmorPenetration = FMath::Max<float>(0.f, sourceArmorPenetration);
	
	FRealCurve* armorPenetrationCurve = characterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float armorPenetrationCoefficient = armorPenetrationCurve->Eval(sourceCombatInterface->GetCreatureLevel());
	
	// ArmorPenetration ignores a percentage of the Target's Armor
	const float effectiveArmor = targetArmor *= (100 - sourceArmorPenetration * armorPenetrationCoefficient) / 100.f;

	FRealCurve* effectiveArmorCurve = characterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float effectiveArmorCoefficient = effectiveArmorCurve->Eval(targetCombatInterface->GetCreatureLevel());
	
	// Armor ignores a percentage of incoming Damage.
	damage *= (100 - effectiveArmor * effectiveArmorCoefficient) / 100.f;

	// Crit logic
	float sourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, evaluationParameters, sourceCriticalHitChance);
	sourceCriticalHitChance = FMath::Max<float>(0.f, sourceCriticalHitChance);

	float sourceCriticalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, evaluationParameters, sourceCriticalDamage);
	sourceCriticalDamage = FMath::Max<float>(0.f, sourceCriticalDamage);

	float targetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, evaluationParameters, targetCriticalHitResistance);
	targetCriticalHitResistance = FMath::Max<float>(0.f, targetCriticalHitResistance);

	const FRealCurve* criticalHitResistanceCurve = characterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float criticalHitResistanceCoefficient = criticalHitResistanceCurve->Eval(targetCombatInterface->GetCreatureLevel());
	
	const bool bCriticalHit = FMath::RandRange(1, 100) <= sourceCriticalHitChance - (targetCriticalHitResistance * criticalHitResistanceCoefficient);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(effectContextHandle, bCriticalHit);

	// if critical hit then increase damage
	damage = bCriticalHit ? 2.f * damage + sourceCriticalDamage : damage; 
	
	const FGameplayModifierEvaluatedData evaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, damage);
	OutExecutionOutput.AddOutputModifier(evaluatedData);
}