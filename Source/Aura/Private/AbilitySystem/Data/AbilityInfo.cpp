// Copyright Stas Company


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbiltyTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilitiesInformation)
	{
		if (AbilityInfo.AbilityTag == AbiltyTag)
		{
			return AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"),
			*AbiltyTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}
