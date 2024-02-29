// Copyright Stas Company


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* characterClassInfo = GetCharacterClassInfo(WorldContextObject);

	for (TSubclassOf<UGameplayAbility> abilityClass : characterClassInfo->CommonAbilities)
	{
		auto abilitySpec = FGameplayAbilitySpec(abilityClass, 1);
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
