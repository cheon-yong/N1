// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/N1HUD.h"
#include "Components/GameFrameworkComponentManager.h"

AN1HUD::AN1HUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AN1HUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AN1HUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AN1HUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
