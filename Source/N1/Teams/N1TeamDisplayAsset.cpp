// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/N1TeamDisplayAsset.h"

#include "Components/MeshComponent.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture.h"
#include "Teams/N1TeamSubsystem.h"
#include "UObject/UObjectIterator.h"

void UN1TeamDisplayAsset::ApplyToMaterial(UMaterialInstanceDynamic* Material)
{
	if (Material)
	{
		for (const auto& KVP : ScalarParameters)
		{
			Material->SetScalarParameterValue(KVP.Key, KVP.Value);
		}

		for (const auto& KVP : ColorParameters)
		{
			Material->SetVectorParameterValue(KVP.Key, FVector(KVP.Value));
		}

		for (const auto& KVP : TextureParameters)
		{
			Material->SetTextureParameterValue(KVP.Key, KVP.Value);
		}
	}
}

void UN1TeamDisplayAsset::ApplyToMeshComponent(UMeshComponent* MeshComponent)
{
	if (MeshComponent)
	{
		for (const auto& KVP : ScalarParameters)
		{
			MeshComponent->SetScalarParameterValueOnMaterials(KVP.Key, KVP.Value);
		}

		for (const auto& KVP : ColorParameters)
		{
			MeshComponent->SetVectorParameterValueOnMaterials(KVP.Key, FVector(KVP.Value));
		}

		const TArray<UMaterialInterface*> MaterialInterfaces = MeshComponent->GetMaterials();
		for (int32 MaterialIndex = 0; MaterialIndex < MaterialInterfaces.Num(); ++MaterialIndex)
		{
			if (UMaterialInterface* MaterialInterface = MaterialInterfaces[MaterialIndex])
			{
				UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
				if (!DynamicMaterial)
				{
					DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(MaterialIndex);
				}

				for (const auto& KVP : TextureParameters)
				{
					DynamicMaterial->SetTextureParameterValue(KVP.Key, KVP.Value);
				}
			}
		}
	}
}

void UN1TeamDisplayAsset::ApplyToNiagaraComponent(UNiagaraComponent* NiagaraComponent)
{
	if (NiagaraComponent)
	{
		for (const auto& KVP : ScalarParameters)
		{
			NiagaraComponent->SetVariableFloat(KVP.Key, KVP.Value);
		}

		for (const auto& KVP : ColorParameters)
		{
			NiagaraComponent->SetVariableLinearColor(KVP.Key, KVP.Value);
		}

		for (const auto& KVP : TextureParameters)
		{
			UTexture* Texture = KVP.Value;
			NiagaraComponent->SetVariableTexture(KVP.Key, Texture);
		}
	}
}

void UN1TeamDisplayAsset::ApplyToActor(AActor* TargetActor, bool bIncludeChildActors)
{
	if (TargetActor != nullptr)
	{
		TargetActor->ForEachComponent(bIncludeChildActors, [this](UActorComponent* InComponent)
			{
				if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(InComponent))
				{
					ApplyToMeshComponent(MeshComponent);
				}
				else if (UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(InComponent))
				{
					ApplyToNiagaraComponent(NiagaraComponent);
				}
			});
	}
}

#if WITH_EDITOR
void UN1TeamDisplayAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	for (UN1TeamSubsystem* TeamSubsystem : TObjectRange<UN1TeamSubsystem>())
	{
		TeamSubsystem->NotifyTeamDisplayAssetModified(this);
	}
}
#endif