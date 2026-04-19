#include "plugin.h"
#include "plugin_helpers.h"
#include "plugin_config.h"
#include "hooks/pause_controller/pause_controller.h"

static IPluginSelf* g_self = nullptr;

IPluginSelf* GetSelf() { return g_self; }

#ifndef MODLOADER_BUILD_TAG
#define MODLOADER_BUILD_TAG "0.5"
#endif

static PluginInfo s_pluginInfo = {
	"KeepTicking",
	MODLOADER_BUILD_TAG,
	"AlienX",
	"Suppresses server pause logic to keep the world ticking at all times",
	PLUGIN_INTERFACE_VERSION
};

extern "C" {
__declspec(dllexport) PluginInfo* GetPluginInfo()
{
	return &s_pluginInfo;
}

__declspec(dllexport) bool PluginInit(IPluginSelf* self)
{
	g_self = self;

	LOG_INFO("[KeepTicking] Plugin initializing...");

	KeepTickingConfig::Config::Initialize(self);

	if (!KeepTickingConfig::Config::IsPluginEnabled())
	{
		LOG_INFO("[KeepTicking] Disabled in config — skipping initialization");
		return true;
	}

	auto* hooks = self->hooks;
	if (hooks && hooks->Hooks)
		Hooks::PauseController::Install(self->scanner, hooks->Hooks);

	return true;
}

__declspec(dllexport) void PluginShutdown()
{
	LOG_INFO("[KeepTicking] Shutting down...");

	auto* hooks = g_self ? g_self->hooks : nullptr;
	if (hooks && hooks->Hooks)
		Hooks::PauseController::Uninstall(hooks->Hooks);

	g_self = nullptr;
}
} // extern "C"
