#pragma once

#include "N1CharacterPartTypes.h"
#include "N1CosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "N1PawnComponent_CharacterParts.generated.h"

class UN1PawnComponent_CharacterParts;

USTRUCT()
struct FN1AppliedCharacterPartEntry
{
	GENERATED_BODY()
	
	UPROPERTY()
	FN1CharacterPart Part;

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FN1CharacterPartList
{
	GENERATED_BODY()
	
	FN1CharacterPartList()
		: OwnerComponent(nullptr) {}

public:

	FN1CharacterPartHandle AddEntry(FN1CharacterPart NewPart);

	bool SpawnActorForEntry(FN1AppliedCharacterPartEntry& Entry);

	FGameplayTagContainer CollectCombinedTags() const;

	void RemoveEntry(FN1CharacterPartHandle Handle);

	void DestroyActorForEntry(FN1AppliedCharacterPartEntry& Entry);

	void SetOwnerComponent(UN1PawnComponent_CharacterParts* InOwnerComponent)
	{
		OwnerComponent = InOwnerComponent;
	}

	UPROPERTY()
	TArray<FN1AppliedCharacterPartEntry> Entries;

	UPROPERTY()
	TObjectPtr<UN1PawnComponent_CharacterParts> OwnerComponent;
	int32 PartHandleCounter = 0;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class UN1PawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	UN1PawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnRegister() override;

	FN1CharacterPartHandle AddCharacterPart(const FN1CharacterPart& NewPart);

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	void BroadcastChanged();

	void RemoveCharacterPart(FN1CharacterPartHandle Handle);


public:
	UPROPERTY()
	FN1CharacterPartList CharacterPartList;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FN1AnimBodyStyleSelectionSet BodyMeshes;
};