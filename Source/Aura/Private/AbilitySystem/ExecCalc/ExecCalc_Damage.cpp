// Copyright Stas Company


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
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
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);	
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

	// Get Damage set by Caller Magnitude
	float damage = spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	// Capture BlockChance on Target, and determine if there was a successful Block

	float targetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, evaluationParameters, targetBlockChance);
	targetBlockChance = FMath::Max<float>(0.f, targetBlockChance);
	
	const bool bBlocked = FMath::RandRange(1, 100) <= targetBlockChance;
	// if Block, halve the damage.
	damage = bBlocked ? UKismetMathLibrary::SafeDivide(damage, 2.f) : damage; 
	
	float targetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, evaluationParameters, targetArmor);
	targetArmor = FMath::Max<float>(0.f, targetArmor);

	float sourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, evaluationParameters, sourceArmorPenetration);
	sourceArmorPenetration = FMath::Max<float>(0.f, sourceArmorPenetration);

	const UCharacterClassInfo* characterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(sourceAvatar);
	FRealCurve* armorPenetrationCurve = characterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float armorPenetrationCoefficient = armorPenetrationCurve->Eval(sourceCombatInterface->GetCreatureLevel());
	
	// ArmorPenetration ignores a percentage of the Target's Armor
	const float effectiveArmor = targetArmor *= (100 - sourceArmorPenetration * 0.25f) / 100.f;

	FRealCurve* effectiveArmorCurve = characterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float effectiveArmorCoefficient = effectiveArmorCurve->Eval(targetCombatInterface->GetCreatureLevel());
	
	// Armor ignores a percentage of incoming Damage.
	damage *= (100 - effectiveArmor * 0.333f) / 100.f;
	
	const FGameplayModifierEvaluatedData evaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, damage);
	OutExecutionOutput.AddOutputModifier(evaluatedData);
}