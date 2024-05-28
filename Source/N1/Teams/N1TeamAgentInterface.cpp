// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/N1TeamAgentInterface.h"

#include "N1LogChannels.h"
#include "UObject/ScriptInterface.h"
// Add default functionality here for any IN1TeamAgentInterface functions that are not pure virtual.

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1TeamAgentInterface)

UN1TeamAgentInterface::UN1TeamAgentInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IN1TeamAgentInterface::ConditionalBroadcastTeamChanged(TScriptInterface<IN1TeamAgentInterface> This, FGenericTeamId OldTeamID, FGenericTeamId NewTeamID)
{
	if (OldTeamID != NewTeamID)
	{
		const int32 OldTeamIndex = GenericTeamIdToInteger(OldTeamID);
		const int32 NewTeamIndex = GenericTeamIdToInteger(NewTeamID);

		UObject* ThisObj = This.GetObject();
		//UE_LOG(LogN1, Verbose, TEXT("[%s] %s assigned team %d"), *GetClientServerContextString(ThisObj), *GetPathNameSafe(ThisObj), NewTeamIndex);

		This.GetInterface()->GetTeamChangedDelegateChecked().Broadcast(ThisObj, OldTeamIndex, NewTeamIndex);
	}
}
