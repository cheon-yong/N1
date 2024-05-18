// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_WorldAction.h"

void UGameFeatureAction_WorldAction::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			AddToWorld(WorldContext, Context);
		}
	}
}
