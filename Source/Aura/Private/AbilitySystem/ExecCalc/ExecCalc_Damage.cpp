// Copyright Stas Company


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);	
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
	
	float armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, evaluationParameters, armor);
	armor = FMath::Max<float>(0.f, armor);
	++armor;
	
	const FGameplayModifierEvaluatedData evaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, armor);
	OutExecutionOutput.AddOutputModifier(evaluatedData);
}