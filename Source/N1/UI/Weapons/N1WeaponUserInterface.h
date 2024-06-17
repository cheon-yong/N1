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

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UN1WeaponInstance* OldWeapon, UN1WeaponInstance* NewWeapon);

private:
	void RebuildWidgetFromWeapon();

private:
	UPROPERTY(Transient)
	TObjectPtr<UN1WeaponInstance> CurrentInstance;
};
