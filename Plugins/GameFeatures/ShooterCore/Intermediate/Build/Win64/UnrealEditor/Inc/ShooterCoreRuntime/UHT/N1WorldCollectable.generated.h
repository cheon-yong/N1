// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "N1WorldCollectable.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SHOOTERCORERUNTIME_N1WorldCollectable_generated_h
#error "N1WorldCollectable.generated.h already included, missing '#pragma once' in N1WorldCollectable.h"
#endif
#define SHOOTERCORERUNTIME_N1WorldCollectable_generated_h

#define FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAN1WorldCollectable(); \
	friend struct Z_Construct_UClass_AN1WorldCollectable_Statics; \
public: \
	DECLARE_CLASS(AN1WorldCollectable, AActor, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/ShooterCoreRuntime"), NO_API) \
	DECLARE_SERIALIZER(AN1WorldCollectable) \
	virtual UObject* _getUObject() const override { return const_cast<AN1WorldCollectable*>(this); }


#define FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AN1WorldCollectable(AN1WorldCollectable&&); \
	AN1WorldCollectable(const AN1WorldCollectable&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AN1WorldCollectable); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AN1WorldCollectable); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(AN1WorldCollectable) \
	NO_API virtual ~AN1WorldCollectable();


#define FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_18_PROLOG
#define FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_21_INCLASS_NO_PURE_DECLS \
	FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SHOOTERCORERUNTIME_API UClass* StaticClass<class AN1WorldCollectable>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_unreal_workspace_N1_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_N1WorldCollectable_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
