// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "N1InputConfig.h"
#include "N1EnhancedInputComponent.generated.h"


class UEnhancedInputLocalPlayerSubsystem;

/**
 * 
 */
UCLASS()
class N1_API UN1EnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UN1EnhancedInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	template <class UserClass, typename FuncType>
	void BindNativeAction(const UN1InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UN1InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
	void AddInputMappings(const UN1InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
};

template <class UserClass, typename FuncType>
void UN1EnhancedInputComponent::BindNativeAction(const UN1InputConfig* InputConfig, const FGameplayTag&
	InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UN1EnhancedInputComponent::BindAbilityActions(const UN1InputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	for (const FN1InputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered,
					Object, PressedFunc, Action.InputTag).GetHandle());
			}
			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed,
					Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
