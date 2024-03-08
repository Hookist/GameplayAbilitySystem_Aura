// Copyright Stas Company


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	if (!playerController) return nullptr;

	if (auto auraHUD = Cast<AAuraHUD>(playerController->GetHUD()))
	{
		auto playerState = playerController->GetPlayerState<AAuraPlayerState>();
		auto aSC = playerState->GetAbilitySystemComponent();
		UAttributeSet* attributeSet = playerState->GetAttributeSet();
		const FWidgetControllerParams WidgetControllerParams(playerController, playerState, aSC, attributeSet);
		return auraHUD->GetOverlayWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	if (!playerController) return nullptr;

	if (auto auraHUD = Cast<AAuraHUD>(playerController->GetHUD()))
	{
		auto playerState = playerController->GetPlayerState<AAuraPlayerState>();
		auto aSC = playerState->GetAbilitySystemComponent();
		UAttributeSet* attributeSet = playerState->GetAttributeSet();
		const FWidgetControllerParams WidgetControllerParams(playerController, playerState, aSC, attributeSet);
		return auraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* avatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* classInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo classDefaultInfo = classInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle primaryAttributesContextHandle = ASC->MakeEffectContext();
	primaryAttributesContextHandle.AddSourceObject(avatarActor);
 	const FGameplayEffectSpecHandle primaryAttributesSpecHandle = ASC->MakeOutgoingSpec(classDefaultInfo.PrimaryAttributes, Level, primaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*primaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle secondaryAttributesContextHandle = ASC->MakeEffectContext();
	secondaryAttributesContextHandle.AddSourceObject(avatarActor);
	const FGameplayEffectSpecHandle secondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(classInfo->SecondaryAttributes, Level, secondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*secondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle vitalAttributesContextHandle = ASC->MakeEffectContext();
	vitalAttributesContextHandle.AddSourceObject(avatarActor);
	const FGameplayEffectSpecHandle vitalAttributesSpecHandle = ASC->MakeOutgoingSpec(classInfo->VitalityAttributes, Level, vitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*vitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* characterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (characterClassInfo == nullptr) return;
	
	for (TSubclassOf<UGameplayAbility> abilityClass : characterClassInfo->CommonAbilities)
	{
		auto abilitySpec = FGameplayAbilitySpec(abilityClass, 1);
		ASC->GiveAbility(abilitySpec);
	}
	const FCharacterClassDefaultInfo& defaultInfo = characterClassInfo->GetClassDefaultInfo(CharacterClass);
	auto combatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());
	for (TSubclassOf<UGameplayAbility> AbilityClass : defaultInfo.StartupAbilities)
	{
		auto abilitySpec = FGameplayAbilitySpec(AbilityClass, combatInterface->GetCreatureLevel());
		ASC->GiveAbility(abilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const auto auraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (auraGameMode == nullptr) return nullptr;

	return auraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameplayEffectContext* context = EffectContextHandle.Get();
	const FAuraGameplayEffectContext* auraEffectContext = static_cast<const FAuraGameplayEffectContext*>(context);
	if (auraEffectContext)
	{
		return auraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameplayEffectContext* context = EffectContextHandle.Get();
	const FAuraGameplayEffectContext* auraEffectContext = static_cast<const FAuraGameplayEffectContext*>(context);
	if (auraEffectContext)
	{
		return auraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	FAuraGameplayEffectContext* auraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (auraEffectContext)
	{
		auraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	FAuraGameplayEffectContext* auraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (auraEffectContext)
	{
		auraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIngore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams sphereParams;
	sphereParams.AddIgnoredActors(ActorsToIngore);

	TArray<FOverlapResult> overlaps;
	if (const UWorld* world = WorldContextObject->GetWorld())
	{
		world->OverlapMultiByObjectType(overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), sphereParams);

		for (FOverlapResult& overlap : overlaps)
		{
			const bool implementsCombatInterface = overlap.GetActor()->Implements<UCombatInterface>();
			if (!implementsCombatInterface) continue;
			
			const bool isAlive = !ICombatInterface::Execute_IsDead(overlap.GetActor());
			if (isAlive)
			{
				OutOverlappingActors.AddUnique(overlap.GetActor());
			}
		}
	}
	
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	if (!IsValid(FirstActor) || !IsValid(SecondActor))
		return false;
	
	const bool firstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool secondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	const bool firstIsEnemy = FirstActor->ActorHasTag(FName("Enemy"));
	const bool secondIsEnemy = SecondActor->ActorHasTag(FName("Enemy"));

	const bool bothArePlayers = firstIsPlayer && secondIsPlayer;
	const bool bothAreEnemies = firstIsEnemy && secondIsEnemy;
	const bool friends = bothArePlayers || bothAreEnemies;
	return !friends;
}
