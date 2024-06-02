// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Feedback/NumberPops/N1NumberPopComponent.h"

#include "N1NumberPopComponent_NiagaraText.generated.h"

class UN1DamagePopStyleNiagara;
class UNiagaraComponent;
class UObject;

/**
 * 
 */
UCLASS()
class N1_API UN1NumberPopComponent_NiagaraText : public UN1NumberPopComponent
{
	GENERATED_BODY()
	
public:
	UN1NumberPopComponent_NiagaraText(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UN1NumberPopComponent interface
	virtual void AddNumberPop(const FN1NumberPopRequest& NewRequest) override;
	//~End of UN1NumberPopComponent interface

protected:
	TArray<int32> DamageNumberArray;

	/** Style patterns to attempt to apply to the incoming number pops */
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UN1DamagePopStyleNiagara> Style;

	//Niagara Component used to display the damage
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
