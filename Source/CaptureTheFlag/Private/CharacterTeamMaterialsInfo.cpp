// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterTeamMaterialsInfo.h"

bool UCharacterTeamMaterials::FindCharacterTeamMaterials(FGameplayTag TeamTag,	TArray<UMaterialInstance*>& OutCharacterTeamMaterials)
{
	for (FCharacterTeamMaterialsInfo CharacterTeamMaterials : CharacterTeamsMaterials)
	{
		if (CharacterTeamMaterials.TeamTag.MatchesTagExact(TeamTag))
		{
			OutCharacterTeamMaterials = CharacterTeamMaterials.TeamMaterials;
			return true;
		}
	}
	return false;
}
