// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

GCINVENTORYSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogInventorySystem, Log, All);

class FGCInventorySystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
