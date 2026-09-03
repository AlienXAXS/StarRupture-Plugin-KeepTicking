#pragma once

struct IPluginHookUtils;
struct IPluginSelf;
struct IPluginHookScanner;

namespace Hooks::PauseController
{
	// Resolve the UpdatePause address. Callable only from the plugin's
	// OnPluginLoadHooks export — the loader refuses scans made anywhere else.
	void Resolve(IPluginSelf* self, IPluginHookScanner* scanner);

	// Install the detour on whatever Resolve found. Safe to call when the pattern
	// missed; it just logs and does nothing.
	void Install(IPluginHookUtils* hookUtils);

	void Uninstall(IPluginHookUtils* hookUtils);
}
