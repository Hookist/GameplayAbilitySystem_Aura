// Copyright Stas Company


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"


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

	ICombatInterface* combatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (combatInterface)
	{
		const FVector socketLocation = combatInterface->GetCombatSocketLocation();
		FRotator rotation = (ProjectileTargetLocation - socketLocation).Rotation();
		rotation.Pitch = 0.f;
		
		FTransform spawnTransform;
		spawnTransform.SetLocation(socketLocation);
		spawnTransform.SetRotation(rotation.Quaternion());
		
		auto projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			spawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: Give the Projectile a GameplayEffect Spec for causing Damage.
		
		projectile->FinishSpawning(spawnTransform);
	}
}