// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "OnlineEngineInterfaceImpl.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ONLINESUBSYSTEMUTILS_OnlineEngineInterfaceImpl_generated_h
#error "OnlineEngineInterfaceImpl.generated.h already included, missing '#pragma once' in OnlineEngineInterfaceImpl.h"
#endif
#define ONLINESUBSYSTEMUTILS_OnlineEngineInterfaceImpl_generated_h

#define FID_Engine_Plugins_Online_OnlineSubsystemUtils_Source_OnlineSubsystemUtils_Private_OnlineEngineInterfaceImpl_h_18_INCLASS \
private: \
	static void StaticRegisterNativesUOnlineEngineInterfaceImpl(); \
	friend struct Z_Construct_UClass_UOnlineEngineInterfaceImpl_Statics; \
public: \
	DECLARE_CLASS(UOnlineEngineInterfaceImpl, UOnlineEngineInterface, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/OnlineSubsystemUtils"), NO_API) \
	DECLARE_SERIALIZER(UOnlineEngineInterfaceImpl)


#define FID_Engine_Plugins_Online_OnlineSubsystemUtils_Source_OnlineSubsystemUtils_Private_OnlineEngineInterfaceImpl_h_18_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UOnlineEngineInterfaceImpl(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UOnlineEngineInterfaceImpl) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UOnlineEngineInterfaceImpl); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UOnlineEngineInterfaceImpl); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UOnlineEngineInterfaceImpl(UOnlineEngineInterfaceImpl&&); \
	UOnlineEngineInterfaceImpl(const UOnlineEngineInterfaceImpl&); \
public: \
	NO_API virtual ~UOnlineEngineInterfaceImpl();


#define FID_Engine_Plugins_Online_OnlineSubsystemUtils_Source_OnlineSubsystemUtils_Private_OnlineEngineInterfaceImpl_h_15_PROLOG
#define FID_Engine_Plugins_Online_OnlineSubsystemUtils_Source_OnlineSubsystemUtils_Private_OnlineEngineInterfaceImpl_h_18_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Engine_Plugins_Online_OnlineSubsystemUtils_Source_OnlineSubsystemUtils_Private_OnlineEngineInterfaceImpl_h_18_INCLASS \
	FID_Engine_Plugins_Online_OnlineSubsystemUtils_Source_OnlineSubsystemUtils_Private_OnlineEngineInterfaceImpl_h_18_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ONLINESUBSYSTEMUTILS_API UClass* StaticClass<class UOnlineEngineInterfaceImpl>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Engine_Plugins_Online_OnlineSubsystemUtils_Source_OnlineSubsystemUtils_Private_OnlineEngineInterfaceImpl_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
