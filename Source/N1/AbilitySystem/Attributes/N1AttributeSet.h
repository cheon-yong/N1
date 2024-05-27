// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"

#include "N1AttributeSet.generated.h"


class AActor;
class UN1AbilitySystemComponent;
class UObject;
class UWorld;
struct FGameplayEffectSpec;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_SixParams(FN1AttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);

/**
 * 
 */
UCLASS()
class N1_API UN1AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UN1AttributeSet();

	UWorld* GetWorld() const override;

	UN1AbilitySystemComponent* GetN1AbilitySystemComponent() const;
};
