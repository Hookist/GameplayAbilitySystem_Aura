// Copyright Stas Company


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInformation.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArmComponent");
	CameraSpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetupAttachment(CameraSpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();

	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCreatureLevel_Implementation()
{
	const auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	return auraPlayerState->GetCreatureLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	
	auraPlayerState->GetAbilitySystemComponent()->
		InitAbilityActorInfo(auraPlayerState, this);
	AbilitySystemComponent = auraPlayerState->GetAbilitySystemComponent();
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AttributeSet = auraPlayerState->GetAttributeSet();

	if (auto playerController = GetController<APlayerController>())
	{
		if (auto auraHUD = Cast<AAuraHUD>(playerController->MyHUD))
		{
			auraHUD->InitOverlay(playerController, auraPlayerState,
			AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	auraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	
	return auraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	
	return auraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	return auraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	return auraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	auto auraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	auraPlayerState->AddToLevel(InPlayerLevel);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	// TODO: Add SpellPoints to PlayerState
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	// TODO: Add AttributePoints to PlayerState
}
