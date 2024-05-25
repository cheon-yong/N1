// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "N1WeaponUserInterface.generated.h"

class UN1WeaponInstance;
/**
 * 
 */
UCLASS()
class N1_API UN1WeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UN1WeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UN1WeaponInstance* OldWeapon, UN1WeaponInstance* NewWeapon);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(Transient)
	TObjectPtr<UN1WeaponInstance> CurrentInstance;
};
