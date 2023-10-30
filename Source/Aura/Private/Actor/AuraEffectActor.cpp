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

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	auto targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

	if (targetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle effectSpecHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, effectContextHandle);
	targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());
}
