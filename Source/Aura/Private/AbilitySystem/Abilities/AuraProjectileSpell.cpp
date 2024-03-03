// Copyright Stas Company


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIServer) return;


	const FVector socketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
	FRotator rotation = (ProjectileTargetLocation - socketLocation).Rotation();
	//rotation.Pitch = 0.f;
	
	FTransform spawnTransform;
	spawnTransform.SetLocation(socketLocation);
	spawnTransform.SetRotation(rotation.Quaternion());
	
	auto projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		spawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle effectContextHandle = sourceASC->MakeEffectContext();
	effectContextHandle.SetAbility(this);
	effectContextHandle.AddSourceObject(projectile);
	TArray<TWeakObjectPtr<AActor>> actors;
	actors.Add(projectile);
	effectContextHandle.AddActors(actors);
	FHitResult hitResult;
	hitResult.Location = ProjectileTargetLocation;
	effectContextHandle.AddHitResult(hitResult);
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), effectContextHandle);

	const FAuraGameplayTags gameplayTags = FAuraGameplayTags::Get();
	const int32 abilityLevel = GetAbilityLevel();

	for (auto& Pair : DamageTypes)
	{
		const float scaledDamage = Pair.Value.GetValueAtLevel(abilityLevel);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, Pair.Key, scaledDamage);
	}

	projectile->DamageEffectSpecHandle = specHandle;
	projectile->FinishSpawning(spawnTransform);
	
}