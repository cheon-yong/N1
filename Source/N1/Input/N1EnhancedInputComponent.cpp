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
