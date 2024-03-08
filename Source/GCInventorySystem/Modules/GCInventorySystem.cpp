// Copyright Epic Games, Inc. All Rights Reserved.

#include "GCInventorySystem.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Subsystems/GCInventoryGISSubsystems.h"
#endif // WITH_EDITOR

#define LOCTEXT_NAMESPACE "FGCInventorySystemModule"

DEFINE_LOG_CATEGORY(LogInventorySystem);

void FGCInventorySystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterSettings();
}

void FGCInventorySystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UnregisterSettings();
}

void FGCInventorySystemModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "GCInventorySystem",
			LOCTEXT("GCInventorySystemName", "Inventory System"),
			LOCTEXT("GCInventorySystemDescription", "Configure the GCInventorySystem plugin"),
			GetMutableDefault<UGCInventoryGISSubsystems>()
		);
	}
#endif // WITH_EDITOR
}

void FGCInventorySystemModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (auto settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		settingsModule->UnregisterSettings("Project", "Plugins", "GCInventorySystem");
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGCInventorySystemModule, GCInventorySystem)