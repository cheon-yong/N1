// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/N1EnhancedInputComponent.h"

#include "EnhancedInputSubsystems.h"

UN1EnhancedInputComponent::UN1EnhancedInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1EnhancedInputComponent::AddInputMappings(const UN1InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
}

void UN1EnhancedInputComponent::RemoveInputMappings(const UN1InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UN1EnhancedInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
