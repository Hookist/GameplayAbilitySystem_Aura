// Copyright Stas Company


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	int32 OldLevel = Level;
	Level = InLevel;
	OnLevelChanged.Broadcast(Level, OldLevel);
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	int32 OldLevel = Level;
	Level += InLevel;
	OnLevelChanged.Broadcast(Level, OldLevel);
}

void AAuraPlayerState::SetXP(int32 NewXP)
{
	int32 OldXP = XP;
	XP = NewXP;
	OnXPChanged.Broadcast(XP, OldXP);
}

void AAuraPlayerState::AddToXP(int32 XPToAdd)
{
	int32 OldXP = XP;
	XP += XPToAdd;
	OnXPChanged.Broadcast(XP, OldXP);
}

int32 AAuraPlayerState::GetXP() const
{
	return XP;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChanged.Broadcast(Level, OldLevel);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChanged.Broadcast(XP, OldXP);
}