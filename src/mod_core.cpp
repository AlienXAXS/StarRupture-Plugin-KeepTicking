#include "mod_core.h"
#include "plugin_helpers.h"
#include "hooks/tick_logger/tick_logger.h"

void ModCore::Initialize(IPluginScanner* scanner, IPluginHooks* hooks)
{
	LOG_INFO("[KeepTicking] Initializing...");

	if (hooks && hooks->Hooks)
		Hooks::TickLogger::Install(scanner, hooks->Hooks);

	LOG_INFO("[KeepTicking] Initialized");
}

void ModCore::Shutdown()
{
	if (auto* hooks = GetHooks(); hooks && hooks->Hooks)
		Hooks::TickLogger::Uninstall(hooks->Hooks);

	LOG_INFO("[KeepTicking] Shutdown complete");
}
