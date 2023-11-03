// Copyright Stas Company


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	auto targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (targetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle effectSpecHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, effectContextHandle);
	const FActiveGameplayEffectHandle activeEffectHandle = targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());

	const bool bIsInfinite = effectSpecHandle.Data.Get()->Def.Get()->DurationPolicy
		== EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfinityEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(activeEffectHandle, targetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (TSubclassOf<UGameplayEffect> gameplayEffect : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffect);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (TSubclassOf<UGameplayEffect> gameplayEffect : DurationGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffect);
		}
	}
	if (InfinityEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (TSubclassOf<UGameplayEffect> gameplayEffect : InfinityGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffect);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (TSubclassOf<UGameplayEffect> gameplayEffect : InstantGameplayEffectClasses)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffect);
		}
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (TSubclassOf<UGameplayEffect> gameplayEffect : DurationGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffect);
		}
	}
	if (InfinityEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (TSubclassOf<UGameplayEffect> gameplayEffect : InfinityGameplayEffectClass)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffect);
		}
	}
	if (InfinityEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		auto targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(targetASC)) return;

		TArray<FActiveGameplayEffectHandle> handlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> Element : ActiveEffectHandles)
		{
			if (targetASC == Element.Value)
			{
				targetASC->RemoveActiveGameplayEffect(Element.Key, 1);
				handlesToRemove.Add(Element.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : handlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
