// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DefaultEditorMap/TeleportToUserFacingExperience.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/N1TeleportText.h"
#include "Game/N1UserFacingExperienceDefinition.h"
#include "N1LogChannels.h"

// Sets default values
ATeleportToUserFacingExperience::ATeleportToUserFacingExperience()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Init Component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SceneComponent->SetupAttachment(GetRootComponent());

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CapsuleComponent->SetupAttachment(SceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneComponent);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_GunPad"));
	Niagara->SetupAttachment(StaticMesh);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ATeleportToUserFacingExperience::BeginPlay()
{
	Super::BeginPlay();
	
	if (UN1TeleportText* TeleportText = Cast<UN1TeleportText>(Widget->GetWidget()))
	{
		TeleportText->SetExperienceText(UserFacingExperience->TileTitle);
	}
}

void ATeleportToUserFacingExperience::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (APawn* PlayerPawn = Cast<APawn>(OtherActor))
	{
		APlayerController* Controller = Cast<APlayerController>(PlayerPawn->GetController());
		if (Controller->IsLocalPlayerController())
		{
			LoadIntoExperience(Controller);
		}
	}
}

void ATeleportToUserFacingExperience::LoadIntoExperience(APlayerController* PlayerController)
{
	if (UserFacingExperience)
	{
		//UserFacingExperience->
	}
}



