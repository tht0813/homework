// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

DECLARE_LOG_CATEGORY_EXTERN(RPGPort, Log, All);
#define RPLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define RPLOG_S(Verbosity) UE_LOG(RPGPort, Verbosity, TEXT("%s"),*RPLOG_CALLINFO)
#define RPLOG(Verbosity, Format, ...) UE_LOG(RPGPort, Verbosity, TEXT("%s%s"), *RPLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define RPCHECK(Expr, ...) { if(!(Expr)) { RPLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }