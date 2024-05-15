// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportToUserFacingExperience.generated.h"

// forward declaration
class UCapsuleComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UWidgetComponent;
class USceneComponent;
class UN1UserFacingExperienceDefinition;

UCLASS()
class N1_API ATeleportToUserFacingExperience : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportToUserFacingExperience();
	FORCEINLINE void SetUserFacingExperienceDefinition(UN1UserFacingExperienceDefinition* UFE) { UserFacingExperience = UFE; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void LoadIntoExperience(APlayerController* PlayerController);
public:	


protected:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> Widget;

	// Experience
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UN1UserFacingExperienceDefinition> UserFacingExperience;
};
