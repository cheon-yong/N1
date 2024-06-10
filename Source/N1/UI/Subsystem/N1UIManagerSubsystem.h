// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameUIManagerSubsystem.h"
#include "N1UIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UN1UIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
	
public:

	UN1UIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

	FTSTicker::FDelegateHandle TickHandle;
};
