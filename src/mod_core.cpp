#include "mod_core.h"
#include "plugin_helpers.h"
#include "hooks/pause_controller/pause_controller.h"

void ModCore::Initialize(IPluginHooks* hooks)
{
	LOG_INFO("[KeepTicking] Initializing...");

	if (hooks && hooks->Hooks)
		Hooks::PauseController::Install(hooks->Hooks);

	LOG_INFO("[KeepTicking] Initialized");
}

void ModCore::Shutdown()
{
	if (auto* hooks = GetHooks(); hooks && hooks->Hooks)
		Hooks::PauseController::Uninstall(hooks->Hooks);

	LOG_INFO("[KeepTicking] Shutdown complete");
}
