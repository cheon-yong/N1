// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "N1VicinityComponent.generated.h"


class UBoxComponent;
/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class N1_API UN1VicinityComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UN1VicinityComponent(const FObjectInitializer& ObjectInitializer);

	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNow;

	/** The name of this component-implemented feature */
	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
};
