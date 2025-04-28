// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamData.h"

bool UTeamData::FindDataTeamInfo(FGameplayTag TeamTag, FTeamDataInfo& OutDataTeamInfo)
{
	for (FTeamDataInfo TeamInfo : TeamsInfos)
	{
		if (TeamInfo.TeamTag == TeamTag)
		{
			OutDataTeamInfo = TeamInfo;
			return true;
		}
	}
	return false;
}
