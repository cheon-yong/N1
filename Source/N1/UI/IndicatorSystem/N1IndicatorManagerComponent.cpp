// Copyright Epic Games, Inc. All Rights Reserved.

#include "N1IndicatorManagerComponent.h"

#include "IndicatorDescriptor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(N1IndicatorManagerComponent)

UN1IndicatorManagerComponent::UN1IndicatorManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoRegister = true;
	bAutoActivate = true;
}

/*static*/ UN1IndicatorManagerComponent* UN1IndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UN1IndicatorManagerComponent>();
	}

	return nullptr;
}

void UN1IndicatorManagerComponent::AddIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	IndicatorDescriptor->SetIndicatorManagerComponent(this);
	OnIndicatorAdded.Broadcast(IndicatorDescriptor);
	Indicators.Add(IndicatorDescriptor);
}

void UN1IndicatorManagerComponent::RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor)
	{
		ensure(IndicatorDescriptor->GetIndicatorManagerComponent() == this);
	
		OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
		Indicators.Remove(IndicatorDescriptor);
	}
}
