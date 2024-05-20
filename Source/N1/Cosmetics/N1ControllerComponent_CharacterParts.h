#pragma once

#include "N1CharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "N1ControllerComponent_CharacterParts.generated.h"

class UN1PawnComponent_CharacterParts;

USTRUCT()
struct FN1ControllerCharacterPartEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FN1CharacterPart Part;

	FN1CharacterPartHandle Handle;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class UN1ControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UN1ControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FN1CharacterPart& NewPart);

	void AddCharacterPartInternal(const FN1CharacterPart& NewPart);

	UN1PawnComponent_CharacterParts* GetPawnCustomizer() const;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void RemoveAllCharacterParts();

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

public:
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FN1ControllerCharacterPartEntry> CharacterParts;
};