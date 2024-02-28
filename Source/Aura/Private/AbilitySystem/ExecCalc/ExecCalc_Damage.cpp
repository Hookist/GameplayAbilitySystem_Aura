// Copyright Stas Company


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);	
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
}


void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* sourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* targetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* sourceAvatar = sourceASC ? sourceASC->GetAvatarActor() : nullptr;
	const AActor* targetAvatar = targetASC ? targetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters evaluationParameters;
	evaluationParameters.SourceTags = sourceTags;
	evaluationParameters.TargetTags = targetTags;

	// Get Damage set by Caller Magnitude
	float damage = spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	// Capture BlockChance on Target, and determine if there was a successful Block
	// if Block, halve the damage.

	float targetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, evaluationParameters, targetBlockChance);
	targetBlockChance = FMath::Max<float>(0.f, targetBlockChance);

	const bool bBlocked = FMath::RandRange(1, 100) <= targetBlockChance;
	damage = bBlocked ? UKismetMathLibrary::SafeDivide(damage, 2) : damage; 
	// float armor = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, evaluationParameters, armor);
	// armor = FMath::Max<float>(0.f, armor);
	// ++armor;
	
	const FGameplayModifierEvaluatedData evaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, damage);
	OutExecutionOutput.AddOutputModifier(evaluatedData);
}