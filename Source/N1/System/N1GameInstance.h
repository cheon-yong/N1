// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "N1GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UN1GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UN1GameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void Init() override;
	virtual void Shutdown() override;
};
