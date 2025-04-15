// Copyright Stas Company


#include "AbilitySystem/Data/LevelUpInformation.h"

int32 ULevelUpInfo::FindLevelForXP(int32 InXP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 1 Information
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		if (InXP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}

	return Level;
}

float ULevelUpInfo::GetLevelPercentage(int32 InXP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 1 Information
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		if (InXP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}

	float buff1 = static_cast<float>(InXP - LevelUpInformation[Level - 1].LevelUpRequirement);
	float buff2 = static_cast<float>(LevelUpInformation[Level].LevelUpRequirement - LevelUpInformation[Level - 1].LevelUpRequirement);
	
	return buff1 / buff2;
}
