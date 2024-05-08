// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//Unreal Engine Headers////////////////////////////////////////////////////////
#include "CoreMinimal.h"
#include <Engine/World.h>
#include <Engine/Engine.h>
#include <Engine/GameInstance.h>
#include <UObject/ObjectMacros.h>
#include <UObject/UObjectGlobals.h>
#include <UObject/ConstructorHelpers.h>
#include <Misc/Build.h>
#include <Templates/SubclassOf.h>
#include <Templates/Function.h>
#include <ProfilingDebugging/ScopedTimers.h>

//C++ Headers//////////////////////////////////////////////////////////////////
#include <functional>

//Personal Headers/////////////////////////////////////////////////////////////
#include "Common/Enum.h"
#include "Common/Macro.h"
#include "Common/Struct.h"
#include "Common/EffectStruct.h"
#include "Common/Util/UtilPath.h"
#include "Common/Util/UtilEffect.h"

// 월드를 반환한다.
UWorld* JQGetWorld( UObject* InObject = nullptr );

// 게임 인스턴스를 반환한다.
UGameInstance* JQGetGameInstance( UWorld* InWorld = nullptr );

DECLARE_LOG_CATEGORY_EXTERN(LogProjectJQ, Log, All);


