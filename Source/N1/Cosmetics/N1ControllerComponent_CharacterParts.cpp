

#include "N1ControllerComponent_CharacterParts.h"
#include "N1PawnComponent_CharacterParts.h"


UN1ControllerComponent_CharacterParts::UN1ControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void UN1ControllerComponent_CharacterParts::AddCharacterPart(const FN1CharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UN1ControllerComponent_CharacterParts::AddCharacterPartInternal(const FN1CharacterPart& NewPart)
{
	FN1ControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;
	if (UN1PawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

UN1PawnComponent_CharacterParts* UN1ControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		return ControlledPawn->FindComponentByClass<UN1PawnComponent_CharacterParts>();
	}
	return nullptr;

}

void UN1ControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UN1ControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

void UN1ControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UN1PawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FN1ControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UN1ControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (UN1PawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UN1PawnComponent_CharacterParts>() : nullptr)
	{
		for (FN1ControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}
	if (UN1PawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UN1PawnComponent_CharacterParts>() : nullptr)
	{
		for (FN1ControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
