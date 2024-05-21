// Fill out your copyright notice in the Description page of Project Settings.


#include "Cosmetics/N1CosmeticAnimationTypes.h"

USkeletalMesh* FN1AnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
    for (const FN1AnimBodyStyleSelectionEntry& Rule : MeshRules)
    {
        if ((Rule.Mesh) && CosmeticTags.HasAll(Rule.RequiredTags))
        {
            return Rule.Mesh;
        }
    }
    return DefaultMesh;

}

TSubclassOf<UAnimInstance> FN1AnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
    for (const FN1AnimLayerSelectionEntry& Rule : LayerRules)
    {
        if ((Rule.Layer != nullptr) && CosmeticTags.HasAll(Rule.RequiredTags))
            return Rule.Layer;
    }
    return DefaultLayer;
}
