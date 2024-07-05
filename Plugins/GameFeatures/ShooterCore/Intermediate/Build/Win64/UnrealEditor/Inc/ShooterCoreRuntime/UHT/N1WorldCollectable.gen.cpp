// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ShooterCoreRuntime/Public/N1WorldCollectable.h"
#include "N1/Interaction/InteractionOption.h"
#include "N1/Inventory/IPickupable.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeN1WorldCollectable() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AActor();
N1_API UClass* Z_Construct_UClass_UInteractableTarget_NoRegister();
N1_API UClass* Z_Construct_UClass_UPickupable_NoRegister();
N1_API UScriptStruct* Z_Construct_UScriptStruct_FInteractionOption();
N1_API UScriptStruct* Z_Construct_UScriptStruct_FInventoryPickup();
SHOOTERCORERUNTIME_API UClass* Z_Construct_UClass_AN1WorldCollectable();
SHOOTERCORERUNTIME_API UClass* Z_Construct_UClass_AN1WorldCollectable_NoRegister();
UPackage* Z_Construct_UPackage__Script_ShooterCoreRuntime();
// End Cross Module References

// Begin Class AN1WorldCollectable
void AN1WorldCollectable::StaticRegisterNativesAN1WorldCollectable()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AN1WorldCollectable);
UClass* Z_Construct_UClass_AN1WorldCollectable_NoRegister()
{
	return AN1WorldCollectable::StaticClass();
}
struct Z_Construct_UClass_AN1WorldCollectable_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "N1WorldCollectable.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/N1WorldCollectable.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Option_MetaData[] = {
		{ "Category", "N1WorldCollectable" },
		{ "ModuleRelativePath", "Public/N1WorldCollectable.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StaticInventory_MetaData[] = {
		{ "Category", "N1WorldCollectable" },
		{ "ModuleRelativePath", "Public/N1WorldCollectable.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Option;
	static const UECodeGen_Private::FStructPropertyParams NewProp_StaticInventory;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AN1WorldCollectable>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AN1WorldCollectable_Statics::NewProp_Option = { "Option", nullptr, (EPropertyFlags)0x0020088000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AN1WorldCollectable, Option), Z_Construct_UScriptStruct_FInteractionOption, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Option_MetaData), NewProp_Option_MetaData) }; // 3692386441
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AN1WorldCollectable_Statics::NewProp_StaticInventory = { "StaticInventory", nullptr, (EPropertyFlags)0x0020080000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AN1WorldCollectable, StaticInventory), Z_Construct_UScriptStruct_FInventoryPickup, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StaticInventory_MetaData), NewProp_StaticInventory_MetaData) }; // 3781045235
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AN1WorldCollectable_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AN1WorldCollectable_Statics::NewProp_Option,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AN1WorldCollectable_Statics::NewProp_StaticInventory,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AN1WorldCollectable_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_AN1WorldCollectable_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_ShooterCoreRuntime,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AN1WorldCollectable_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_AN1WorldCollectable_Statics::InterfaceParams[] = {
	{ Z_Construct_UClass_UInteractableTarget_NoRegister, (int32)VTABLE_OFFSET(AN1WorldCollectable, IInteractableTarget), false },  // 3989564772
	{ Z_Construct_UClass_UPickupable_NoRegister, (int32)VTABLE_OFFSET(AN1WorldCollectable, IPickupable), false },  // 811290540
};
const UECodeGen_Private::FClassParams Z_Construct_UClass_AN1WorldCollectable_Statics::ClassParams = {
	&AN1WorldCollectable::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AN1WorldCollectable_Statics::PropPointers,
	InterfaceParams,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AN1WorldCollectable_Statics::PropPointers),
	UE_ARRAY_COUNT(InterfaceParams),
	0x008000A5u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AN1WorldCollectable_Statics::Class_MetaDataParams), Z_Construct_UClass_AN1WorldCollectable_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AN1WorldCollectable()
{
	if (!Z_Registration_Info_UClass_AN1WorldCollectable.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AN1WorldCollectable.OuterSingleton, Z_Construct_UClass_AN1WorldCollectable_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AN1WorldCollectable.OuterSingleton;
}
template<> SHOOTERCORERUNTIME_API UClass* StaticClass<AN1WorldCollectable>()
{
	return AN1WorldCollectable::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AN1WorldCollectable);
AN1WorldCollectable::~AN1WorldCollectable() {}
// End Class AN1WorldCollectable

// Begin Registration
struct Z_CompiledInDeferFile_FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AN1WorldCollectable, AN1WorldCollectable::StaticClass, TEXT("AN1WorldCollectable"), &Z_Registration_Info_UClass_AN1WorldCollectable, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AN1WorldCollectable), 2897106124U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_2955962545(TEXT("/Script/ShooterCoreRuntime"),
	Z_CompiledInDeferFile_FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
