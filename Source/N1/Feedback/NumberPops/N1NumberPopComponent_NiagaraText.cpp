// Fill out your copyright notice in the Description page of Project Settings.


#include "Feedback/NumberPops/N1NumberPopComponent_NiagaraText.h"
#include "N1LogChannels.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Feedback/NumberPops/N1DamagePopStyleNiagara.h"

UN1NumberPopComponent_NiagaraText::UN1NumberPopComponent_NiagaraText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UN1NumberPopComponent_NiagaraText::AddNumberPop(const FN1NumberPopRequest& NewRequest)
{
	int32 LocalDamage = NewRequest.NumberToDisplay;

	//Change Damage to negative to differentiate Critial vs Normal hit
	if (NewRequest.bIsCriticalDamage)
	{
		LocalDamage *= -1;
	}

	//Add a NiagaraComponent if we don't already have one
	if (!NiagaraComp)
	{
		NiagaraComp = NewObject<UNiagaraComponent>(GetOwner());
		if (Style != nullptr)
		{
			NiagaraComp->SetAsset(Style->TextNiagara);
			NiagaraComp->bAutoActivate = false;

		}
		NiagaraComp->SetupAttachment(nullptr);
		check(NiagaraComp);
		NiagaraComp->RegisterComponent();
	}


	NiagaraComp->Activate(false);
	NiagaraComp->SetWorldLocation(NewRequest.WorldLocation);

	UE_LOG(LogN1, Log, TEXT("DamageHit location : %s, Damage : %d"), *(NewRequest.WorldLocation.ToString()), LocalDamage);
	//Add Damage information to the current Niagara list - Damage informations are packed inside a FVector4 where XYZ = Position, W = Damage
	TArray<FVector4> DamageList = UNiagaraDataInterfaceArrayFunctionLibrary::GetNiagaraArrayVector4(NiagaraComp, Style->NiagaraArrayName);
	DamageList.Add(FVector4(NewRequest.WorldLocation.X, NewRequest.WorldLocation.Y, NewRequest.WorldLocation.Z, LocalDamage));
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NiagaraComp, Style->NiagaraArrayName, DamageList);
}
