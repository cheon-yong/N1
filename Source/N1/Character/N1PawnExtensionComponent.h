// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "N1PawnExtensionComponent.generated.h"

class UN1PawnData;

/**
 * 
 */
UCLASS()
class N1_API UN1PawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UN1PawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	static const FName NAME_ActorFeatureName;
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }

	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
		FGameplayTag DesiredState) const override;
	virtual void CheckDefaultInitialization() override;
	void SetPawnData(const UN1PawnData* InPawnData);
	void SetupPlayerInputComponent();

	static UN1PawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) {
		return
			(Actor ? Actor->FindComponentByClass<UN1PawnExtensionComponent>() : nullptr);
	}

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

public:
	UPROPERTY(EditInstanceOnly, Category = "N1|Pawn")
	TObjectPtr<const UN1PawnData> PawnData;
};
