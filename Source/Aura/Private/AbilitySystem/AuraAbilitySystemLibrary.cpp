// Copyright Stas Company


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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
	auto auraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (auraGameMode == nullptr) return;

	AActor* avatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* classInfo = auraGameMode->CharacterClassInfo;
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
	auto auraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (auraGameMode == nullptr) return;

	UCharacterClassInfo* characterClassInfo = auraGameMode->CharacterClassInfo;

	for (TSubclassOf<UGameplayAbility> abilityClass : characterClassInfo->CommonAbilities)
	{
		auto abilitySpec = FGameplayAbilitySpec(abilityClass, 1);
		ASC->GiveAbility(abilitySpec);
	}
}
