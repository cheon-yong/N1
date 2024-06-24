// Fill out your copyright notice in the Description page of Project Settings.


#include "System/N1GameInstance.h"
#include <Components/GameFrameworkComponentManager.h>
#include "N1GameplayTags.h"

UN1GameInstance::UN1GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1GameInstance::Init()
{
	Super::Init();
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(N1GameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(N1GameplayTags::InitState_DataAvailable, false, N1GameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(N1GameplayTags::InitState_DataInitialized, false, N1GameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(N1GameplayTags::InitState_GameplayReady, false, N1GameplayTags::InitState_DataInitialized);
	}

}

void UN1GameInstance::Shutdown()
{

	Super::Shutdown();
}
