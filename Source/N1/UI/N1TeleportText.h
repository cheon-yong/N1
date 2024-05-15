// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/N1UserWidget.h"
#include "N1TeleportText.generated.h"

/**
 * 
 */
UCLASS()
class N1_API UN1TeleportText : public UN1UserWidget
{
	GENERATED_BODY()
	
public:
	void SetExperienceText(FText InText);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ExperienceText;
};
